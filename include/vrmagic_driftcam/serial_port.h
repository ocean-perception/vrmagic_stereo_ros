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

  void open(const std::string& serialPortPath) noexcept;

  EBaudRate getBaudRate() const noexcept;

  ECharacterSize getCharSize() const noexcept;

  EStopBits getNumOfStopBits() const noexcept;

  EParity getParity() const noexcept;

  EFlowControl getFlowControl() const noexcept;

  /*
   * Serial port configuration settings.
   */
  void setBaudRate(const EBaudRate baudRate) noexcept;

  void setCharSize(const ECharacterSize charSize) noexcept;

  void setNumOfStopBits(const EStopBits stopBits) noexcept;

  void setParity(const EParity parity) noexcept;

  void setFlowControl(const EFlowControl flowControl) noexcept;

  bool isOpen() const;

  unsigned char readByte(const unsigned int msTimeout = 0) noexcept;

  void read(DataBuffer& dataBuffer, const unsigned int numOfBytes = 0,
            const unsigned int msTimeout = 0) noexcept;
  std::string readLine(
      const unsigned int msTimeout = 0,
      const char lineTerminator = '\n') noexcept;

  void writeByte(const unsigned char dataByte) noexcept;

  void write(const DataBuffer& dataBuffer) noexcept;

  void write(const std::string& dataString) noexcept;

  void sendBreak() noexcept;

  void close() noexcept;

 private:
  boost::asio::io_service io_;
  mutable boost::asio::serial_port serialPort_;

  void timedReadHandler(boost::optional<boost::system::error_code>* a,
                        const boost::system::error_code& b);

};  // class SerialPort
}  // namespace driftcam

#endif /* DRIFTCAM_SERIAL_PORT_H_ */
