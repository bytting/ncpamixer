#ifndef PA_OBJECT_
#define PA_OBJECT_
#include <pulse/pulseaudio.h>
#include <vector>
#include "pa_object_attribute.hpp"
#include "pa_port.hpp"

enum pa_object_t {SINK, INPUT, SOURCE, SOURCE_OUTPUT, CARD};

class PaObject
{
public:
    PaObject() {};
    virtual ~PaObject();
    pa_object_t type;

    uint32_t index;
    char name[255];

    unsigned int channels;
    pa_volume_t volume;
    bool mute;

    uint32_t monitor_index;
    pa_stream *monitor_stream;
    float peak;

    PaPort *active_port = nullptr;
    std::vector<PaPort *> ports;
    void clearPorts();

    PaObjectAttribute *active_profile = nullptr;
    std::vector<PaObjectAttribute *> profiles;
    void clearProfiles();

    pa_operation *(*pa_set_volume)(pa_context *, uint32_t, const pa_cvolume *,
                                   pa_context_success_cb_t, void *);
    pa_operation *(*pa_set_mute)(pa_context *, uint32_t, int,
                                 pa_context_success_cb_t, void *);
    pa_operation *(*pa_move)(pa_context *, uint32_t, uint32_t,
                             pa_context_success_cb_t, void *);
    pa_operation *(*pa_set_profile)(pa_context *, uint32_t, const char *,
                             pa_context_success_cb_t, void *);


    void set_volume(float perc);
    void step_volume(int dir);
    void move(uint32_t dest);
    void toggle_mute();
    void set_profile(const char* profile);

    virtual char *getAppName()
    {
        return nullptr;
    };

    virtual uint32_t getRelation()
    {
        throw - 1;
    };
};

#endif // PA_OBJECT_
