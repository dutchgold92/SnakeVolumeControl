#include "volume_controller.h"

using namespace SnakeVolumeControl;

class PulseAudioSystemVolumeControlAdapter : public SystemVolumeControlAdapter {
public:
    void setVolume(unsigned int percentage) {
        std::string cmd = "amixer -D pulse sset Master ";
        cmd += std::to_string(percentage);
        cmd += "%";

        QProcess cmdProcess;
        cmdProcess.start(QString::fromStdString(cmd));
        cmdProcess.waitForFinished(-1);
    }

    bool isApplicable() {
//        QProcess checkAmixerCmdProcess;
//        checkAmixerCmdProcess.start(QString("amixer"), QStringList() << "--version");
//        checkAmixerCmdProcess.waitForFinished(-1);

//        if (checkAmixerCmdProcess.exitCode() != 0) {
//            std::cout << checkAmixerCmdProcess.exitCode() << std::endl;
//            return false;
//        }

//        QProcess checkPactlCmdProcess;
//        checkPactlCmdProcess.start(QString("pactl --version"));
//        checkPactlCmdProcess.waitForFinished(-1);

//        if (checkPactlCmdProcess.exitCode() != 0) {
//            return false;
//        }

        return true;
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
