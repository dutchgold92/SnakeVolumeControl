#ifndef VOLUME_CONTROLLER_H
#define VOLUME_CONTROLLER_H

#include <exception>
#include <iostream>
#include <QProcess>

namespace SnakeVolumeControl {
    class SystemVolumeControlAdapter {
    public:
        virtual ~SystemVolumeControlAdapter() {}
        virtual void setVolume(unsigned int percentage) = 0;
        virtual bool isApplicable() = 0;
    };

    class VolumeControlAdapterNotAvailableException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "No applicable volume control adapter defined";
        }
    };

    SystemVolumeControlAdapter* getVolumeControlAdapter() throw (VolumeControlAdapterNotAvailableException);
}

#endif // VOLUME_CONTROLLER_H
