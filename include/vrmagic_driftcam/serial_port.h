#ifndef DRIFTCAM_SERIAL_PORT_H_
#define DRIFTCAM_SERIAL_PORT_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <vrmagic_driftcam/iserial_port.h>

namespace Driftcam {

struct SpConfig {
  // ------- Serial Port Config -------
  std::string sp_path;
  int sp_baud_rate;
  int sp_char_size;
  int sp_stop_bits;
  std::string sp_parity;
  std::string sp_flow_control;
  int sp_timeout;
};

class SerialPort : public ISerialPort, private boost::noncopyable {
 public:
  SerialPort() : io_(), serialPort_(io_) {}

  ~SerialPort() {}

  void open(const std::string& serialPortPath) throw(AlreadyOpen, OpenFailed,
                                                     UnsupportedBaudRate,
                                                     std::invalid_argument);

  EBaudRate getBaudRate() const throw(NotOpen, std::runtime_error);

  ECharacterSize getCharSize() const throw(NotOpen, std::runtime_error);

  EStopBits getNumOfStopBits() const throw(NotOpen, std::runtime_error);

  EParity getParity() const throw(NotOpen, std::runtime_error);

  EFlowControl getFlowControl() const throw(NotOpen, std::runtime_error);

  /*
   * Serial port configuration settings.
   */
  void setBaudRate(const EBaudRate baudRate) throw(UnsupportedBaudRate, NotOpen,
                                                   std::invalid_argument,
                                                   std::runtime_error);

  void setCharSize(const ECharacterSize charSize) throw(NotOpen,
                                                        std::invalid_argument,
                                                        std::runtime_error);

  void setNumOfStopBits(const EStopBits stopBits) throw(NotOpen,
                                                        std::invalid_argument,
                                                        std::runtime_error);

  void setParity(const EParity parity) throw(NotOpen, std::invalid_argument,
                                             std::runtime_error);

  void setFlowControl(const EFlowControl flowControl) throw(
      NotOpen, std::invalid_argument, std::runtime_error);

  bool isOpen() const;

  unsigned char readByte(const unsigned int msTimeout = 0) throw(
      NotOpen, ReadTimeout, std::runtime_error);

  void read(DataBuffer& dataBuffer, const unsigned int numOfBytes = 0,
            const unsigned int msTimeout = 0) throw(NotOpen, ReadTimeout,
                                                    std::runtime_error);
  std::string readLine(
      const unsigned int msTimeout = 0,
      const char lineTerminator = '\n') throw(NotOpen, ReadTimeout,
                                              std::runtime_error);

  void writeByte(const unsigned char dataByte) throw(NotOpen,
                                                     std::runtime_error);

  void write(const DataBuffer& dataBuffer) throw(NotOpen, std::runtime_error);

  void write(const std::string& dataString) throw(NotOpen, std::runtime_error);

  void sendBreak() throw(NotOpen, std::runtime_error);

  void close() throw(NotOpen);

 private:
  boost::asio::io_service io_;
  mutable boost::asio::serial_port serialPort_;

  void timedReadHandler(boost::optional<boost::system::error_code>* a,
                        const boost::system::error_code& b);

};  // class SerialPort
}  // namespace driftcam

#endif /* DRIFTCAM_SERIAL_PORT_H_ */