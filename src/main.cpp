#include <getopt.h>
#include <wordexp.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <vector>

#include <audio.hpp>
#include <config.hpp>
#include <version.hpp>

#include <ui/ui.hpp>

void version()
{
    if (strlen(GIT_VERSION) > 0) {
        printf("ncpamixer git v%s\n", &GIT_VERSION[0]);
    } else {
        printf("ncpamixer v%s\n", &FALLBACK_VERSION[0]);
    }

    printf("Build type: %s\n", &BUILD_TYPE[0]);
    printf("Build date: %s\n", &BUILD_DATE[0]);
    printf("\n");

}

void help()
{
    version();

    printf("-c --config=CONFIG_FILE     Set custom location for config\n");
    printf("-h --help                   Print this help screen\n");
    printf("-v --version                Print version info\n");
}

int main(int argc, char *argv[])
{
    static const struct option longOpts[] = {
        { "version", no_argument, nullptr, 'v' },
        { "help", no_argument, nullptr, 'h' },
        { "config", required_argument, nullptr, 'c' },
        { nullptr, 0, nullptr, 0 }
    };

    int c;
    int longIndex = 0;

    std::string conf;

    while ((c = getopt_long(argc, argv, "vhc:", &longOpts[0], &longIndex)) != -1) {
        switch (c) {
            case 'v':
                version();
                return 0;

            case 'h':
                help();
                return 0;

            case 'c':
                if(realpath(optarg, const_cast<char*>(conf.c_str())) == nullptr) {
                    fprintf(
                        stderr,
                        "realpath error code: %d, %s",
                        errno,
                        strerror(errno)
                    );
                }
                break;

            case '?':
                return 0;

            default:
                break;
        }
    }

    config.init(conf.c_str());
    if (audio != nullptr) {
        audio->init();

        if (ui.init() > 0) {
            ui.run();
        }

        return 0;
    }

    fprintf(stderr, "No audio engine specified!\n");

    return 1;
}