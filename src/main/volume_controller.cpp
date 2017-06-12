#include "include/volume_controller.h"

using namespace SnakeVolumeControl;

class PulseAudioSystemVolumeControlAdapter : public SystemVolumeControlAdapter {
public:
    void setVolume(unsigned int percentage) override {
        std::string cmd = "amixer -D pulse sset Master ";
        cmd += std::to_string(percentage);
        cmd += "%";

        QProcess cmdProcess;
        cmdProcess.start(QString::fromStdString(cmd));
        cmdProcess.waitForFinished(-1);
    }

    bool isApplicable() override {
        int amixerAvailable = system("which amixer > /dev/null 2>&1");
        int pactlAvailable = system("which pactl > /dev/null 2>&1");

        return ((amixerAvailable == pactlAvailable) && (amixerAvailable == 0));
    }
};

namespace SnakeVolumeControl {
    SystemVolumeControlAdapter* getVolumeControlAdapter() throw (VolumeControlAdapterNotAvailableException) {
        SystemVolumeControlAdapter *result = new PulseAudioSystemVolumeControlAdapter();

        if (result->isApplicable()) {
            return result;
        } else {
            delete result;
            throw VolumeControlAdapterNotAvailableException();
        }
    }
}
