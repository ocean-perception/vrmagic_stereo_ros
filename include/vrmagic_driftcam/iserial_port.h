#ifndef DRIFTCAM_I_SERIAL_PORT_H_
#define DRIFTCAM_I_SERIAL_PORT_H_

#include <stdexcept>
#include <string>
#include <vector>

namespace Driftcam {

/*
 * The allowed set of baud rates.
 */
enum EBaudRate {
  BAUD_50,
  BAUD_75,
  BAUD_110,
  BAUD_134,
  BAUD_150,
  BAUD_200,
  BAUD_300,
  BAUD_600,
  BAUD_1200,
  BAUD_1800,
  BAUD_2400,
  BAUD_4800,
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200,
  BAUD_230400,
//
// Note: B460800 is defined on Linux but not on Mac OS
// X. What about other operating systems ?
//
#ifdef __linux__
  BAUD_460800,
#endif
  BAUD_INVALID,
  BAUD_DEFAULT = BAUD_57600
};

/*
 * The allowed character sizes.
 */
enum ECharacterSize {
  CHAR_SIZE_5,  //!< 5 bit characters.
  CHAR_SIZE_6,  //!< 6 bit characters.
  CHAR_SIZE_7,  //!< 7 bit characters.
  CHAR_SIZE_8,  //!< 8 bit characters.
  CHAR_SIZE_INVALID,
  CHAR_SIZE_DEFAULT = CHAR_SIZE_8
};

/*
 * The allowed numbers of stop bits.
 */
enum EStopBits {
  STOP_BITS_1,  //! 1 stop bit.
  STOP_BITS_2,  //! 2 stop bits.
  STOP_BITS_INVALID,
  STOP_BITS_DEFAULT = STOP_BITS_1
};

/*
 * The allowed parity checking modes.
 */
enum EParity {
  PARITY_EVEN,  //!< Even parity.
  PARITY_ODD,   //!< Odd parity.
  PARITY_NONE,  //!< No parity i.e. parity checking disabled.
  PARITY_INVALID,
  PARITY_DEFAULT = PARITY_NONE
};

/*
 * The allowed flow control modes.
 */
enum EFlowControl {
  FLOW_CONTROL_HARD,
  FLOW_CONTROL_SOFT,
  FLOW_CONTROL_NONE,
  FLOW_CONTROL_INVALID,
  FLOW_CONTROL_DEFAULT = FLOW_CONTROL_NONE
};

/*
 * ISerialPort
 * Interface for a serial port device, supporting most common operations.
 */
class ISerialPort {
 public:
  typedef std::vector<unsigned char> DataBuffer;

  

  class NotOpen : public std::logic_error {
   public:
    NotOpen() : logic_error("Serial port is not open") {}
    NotOpen(const std::string& whatArg) : logic_error(whatArg) {}
  };

  class OpenFailed : public std::runtime_error {
   public:
    OpenFailed() : runtime_error("Serial port failed to open") {}
    OpenFailed(const std::string& whatArg) : runtime_error(whatArg) {}
  };

  class AlreadyOpen : public std::logic_error {
   public:
    AlreadyOpen() : logic_error("Serial port was already open") {}
    AlreadyOpen(const std::string& whatArg) : logic_error(whatArg) {}
  };

  class UnsupportedBaudRate : public std::runtime_error {
   public:
    UnsupportedBaudRate() : runtime_error("Unsupported baud rate") {}
    UnsupportedBaudRate(const std::string& whatArg) : runtime_error(whatArg) {}
  };

  class ReadTimeout : public std::runtime_error {
   public:
    ReadTimeout() : runtime_error("Serial port read timeout") {}
    ReadTimeout(const std::string& whatArg) : runtime_error(whatArg) {}
  };

  /*
   * Helper functions.
   */
  static EBaudRate baudRateFromInteger(const unsigned int baudRate) noexcept {
    switch (baudRate) {
      case 50:
        return BAUD_50;
      case 75:
        return BAUD_75;
      case 110:
        return BAUD_110;
      case 134:
        return BAUD_134;
      case 150:
        return BAUD_150;
      case 200:
        return BAUD_200;
      case 300:
        return BAUD_300;
      case 600:
        return BAUD_600;
      case 1200:
        return BAUD_1200;
      case 1800:
        return BAUD_1800;
      case 2400:
        return BAUD_2400;
      case 4800:
        return BAUD_4800;
      case 9600:
        return BAUD_9600;
      case 19200:
        return BAUD_19200;
      case 38400:
        return BAUD_38400;
      case 57600:
        return BAUD_57600;
      case 115200:
        return BAUD_115200;
      case 230400:
        return BAUD_230400;
#ifdef __linux__
      case 460800:
        return BAUD_460800;
#endif
    }
  }

  static ECharacterSize charSizeFromInteger(const unsigned int charSize) noexcept {
    switch (charSize) {
      case 5:
        return CHAR_SIZE_5;
      case 6:
        return CHAR_SIZE_6;
      case 7:
        return CHAR_SIZE_7;
      case 8:
        return CHAR_SIZE_8;
        
    }
  }

  static EStopBits numOfStopBitsFromInteger(const unsigned int stopBits) noexcept {
    switch (stopBits) {
      case 1:
        return STOP_BITS_1;
      case 2:
        return STOP_BITS_2;
    }
  }

  static EParity parityFromString(const std::string& parity) noexcept {
    if (parity == "EVEN")
      return PARITY_EVEN;
    else if (parity == "ODD")
      return PARITY_ODD;
    else if (parity == "NONE")
      return PARITY_NONE;
  }

  static EFlowControl flowControlFromString(
      const std::string& flowControl) noexcept {
    if (flowControl == "HARD")
      return FLOW_CONTROL_HARD;
    else if (flowControl == "SOFT")
      return FLOW_CONTROL_SOFT;
    else if (flowControl == "NONE")
      return FLOW_CONTROL_NONE;
  }

  /*
   * Open a serial port.
   */
  virtual void open(const std::string& serialPortPath) noexcept = 0;

  /*
   * Getters for serial port configuration.
   */
  virtual EBaudRate getBaudRate() const noexcept = 0;

  virtual ECharacterSize getCharSize() const noexcept = 0;

  virtual EStopBits getNumOfStopBits() const noexcept= 0;

  virtual EParity getParity() const noexcept = 0;

  virtual EFlowControl getFlowControl() const noexcept = 0;

  /*
   * Serial port configuration settings.
   */
  virtual void setBaudRate(const EBaudRate baudRate) noexcept = 0;

  virtual void setCharSize(const ECharacterSize charSize) noexcept = 0;

  virtual void setNumOfStopBits(const EStopBits stopBits) noexcept = 0;

  virtual void setParity(const EParity parity) noexcept = 0;

  virtual void setFlowControl(const EFlowControl flowControl) noexcept = 0;

  virtual bool isOpen() const = 0;

  /**
   * Read a single byte from the serial port. If no data is
   * available in the specified number of milliseconds (msTimeout),
   * then this method will throw ReadTimeout exception. If msTimeout
   * is 0, then this method will block till data is available.
   */
  virtual unsigned char readByte(const unsigned int msTimeout = 0) noexcept = 0;

  /**
   * Read the specified number of bytes from the serial port. The
   * method will timeout if no data is received in the specified
   * number of milliseconds (msTimeout). If msTimeout is 0, then
   * this method will block till all requested bytes are
   * received. If numOfBytes is zero, then this method will keep
   * reading data till no more data is available at the serial
   * port. In all cases, all read data is available in dataBuffer on
   * return from this method.
   */
  virtual void read(DataBuffer& dataBuffer, const unsigned int numOfBytes = 0,
                    const unsigned int msTimeout =
                        0) noexcept = 0;

  /**
   * Read a line of characters from the serial port.
   */
  virtual std::string readLine(
      const unsigned int msTimeout = 0,
      const char lineTerminator = '\n') noexcept = 0;

  /**
   * Send a single byte to the serial port.
   *
   * @throw NotOpen Thrown if this method is called while the serial
   * port is not open.
   */
  virtual void writeByte(const unsigned char dataByte) noexcept = 0;

  /**
   * Write the data from the specified vector to the serial port.
   */
  virtual void write(const DataBuffer& dataBuffer) noexcept = 0;

  /**
   * Write a string to the serial port.
   */
  virtual void write(const std::string& dataString) noexcept = 0;

  /**
   * Send a break sequence to the serial port.
   */
  virtual void sendBreak() noexcept = 0;

  /*
   * Close the serial port.
   */
  virtual void close() noexcept = 0;

  virtual ~ISerialPort() {}
};
};  // namespace driftcam

#endif /* DRIFTCAM_I_SERIAL_PORT_H_ */
