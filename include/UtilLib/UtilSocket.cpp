#include "stdafx.h"
#include "UtilSocket.h"

utilSocket::utilSocket(int messageLength)
{
    _msg_len = messageLength;
    _pSocket = new boost::asio::ip::tcp::socket(_io_service);
}

utilSocket::~utilSocket() {
    _pSocket->get_io_service().reset();
    _pSocket->get_io_service().stop();
}

void utilSocket::setAddress(std::string ipaddr, std::string port) {
    _ip_addr = ipaddr;
    _port = port;
}

bool utilSocket::connect() {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(_ip_addr),stoi(_port));
    boost::system::error_code err;
    _pSocket->connect(endpoint, err);

    // Connection failed
    if (err != boost::system::errc::success) {
        return false;
    }

    return true;
}

void utilSocket::disconnect() {
    _pSocket->close();
}

bool utilSocket::sendMessage(std::string& message) {
    _pSocket->send(boost::asio::buffer(message)); // Not tested
    return true;
}

bool utilSocket::recvMessage(std::string& message) {
    boost::asio::streambuf sb;
    boost::system::error_code err;

    std::size_t bytes_transferred = boost::asio::read(*_pSocket, sb, boost::asio::transfer_exactly(_msg_len), err);

    if (err == boost::asio::error::eof) {
        this->disconnect();
        return false;
    }

    sb.commit(bytes_transferred);

    boost::asio::streambuf::const_buffers_type bufs = sb.data();
    std::string retStr(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) +_msg_len);
    message = retStr;
    return true;
}

bool utilSocket::_flushBuffer() {


    return true;
}

bool utilSocket::recvMessageTimeout(std::string& message, double timeout_msec) {
    // This function will always wait timeout_sec before returning, and will
    // return the most recent socket message, throwing away all old messsages.
    //qDebug() << "Reading...";
    boost::asio::streambuf sbflush;
    boost::asio::streambuf sb;
    boost::system::error_code err;

    // Setup timeout
    boost::optional<boost::system::error_code> timer_result;
    boost::system::error_code timer_err;
    boost::asio::deadline_timer timer(_pSocket->get_io_service());
    timer.expires_from_now(boost::posix_time::milliseconds(timeout_msec));
    timer.async_wait(boost::bind(&utilSocket::_setResult, this, &timer_result, timer_err));

    // Setup read
    boost::optional<boost::system::error_code> read_result;
    int cnt = 0;
    boost::asio::async_read(*_pSocket, sb, boost::asio::transfer_exactly(_msg_len), \
                            boost::bind(&utilSocket::_setResult, this, &read_result, err));
    _pSocket->get_io_service().reset();
    std::string retStr;
    //qDebug() << "Checking timer...";
    while (_pSocket->get_io_service().run_one()) {

        if (timer_result) {
            _pSocket->cancel();
            //qDebug() << "Timer done!";
            break;
        }
        if (read_result) {
            if (sb.size() >= _msg_len) {
                // Save value
                boost::asio::streambuf::const_buffers_type bufs = sb.data();
                std::string temp(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));
                boost::trim_if(temp, boost::is_any_of(std::string("\0",1)));

                if (temp.size() >= _msg_len) {
                    retStr = temp;
                }
            }

            if (sb.size() > 0) {
                sb.consume(sb.size());
            }

            boost::asio::async_read(*_pSocket, sb, boost::asio::transfer_exactly(_msg_len), \
                                    boost::bind(&utilSocket::_setResult, this, &read_result, err));


            //qDebug() << " Read done!";
        }
        cnt += 1;

    }
    _pSocket->get_io_service().reset();

    if (err == boost::asio::error::eof) {
        this->disconnect();
        return false;
    }
    //qDebug() << "Buffer size:" << sb.size();
    /*if (sb.size() >= mMsgLen) {
        boost::asio::streambuf::const_buffers_type bufs = sb.data();
        retStr = std::string(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));

    }else if (sb.size() == 0 && sbflush.size() > 0) {
        boost::asio::streambuf::const_buffers_type bufs = sb.data();
        retStr = std::string(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));
    }*/
    //boost::trim_right_if(retStr, boost::is_any_of(std::string("\0",1))); //  Remove null characters
    // Sanity check
    if (retStr.size() < _msg_len) {
        //qDebug() << "utilSocket: Message too short." << retStr.size();
        return false;
    }

    //qDebug() << "Message size:" << retStr.size();
    message = retStr.substr(retStr.size()-_msg_len, retStr.size()-1); // Pull most recent message
    return true;
}

void utilSocket::_setResult(boost::optional<boost::system::error_code> *a, boost::system::error_code b) {
    a->reset(b);
}


/*



bool utilSocket::recvMessageTimeout(std::string& message, double timeout_msec) {
    // This function will always wait timeout_sec before returning, and will
    // return the most recent socket message, throwing away all old messsages.
    qDebug() << "0";
    boost::asio::streambuf sb;
    boost::system::error_code err;

    // Setup timeout
    boost::optional<boost::system::error_code> timer_result;
    boost::system::error_code timer_err;
    boost::asio::deadline_timer timer(mpSocket->get_io_service());
    timer.expires_from_now(boost::posix_time::milliseconds(timeout_msec));
    timer.async_wait(boost::bind(&utilSocket::set_result, this, &timer_result, timer_err));

    // Setup read
    boost::optional<boost::system::error_code> read_result;
    int cnt = 0;
    qDebug() << "1";
    boost::asio::async_read(*mpSocket, sb, boost::asio::transfer_exactly(mMsgLen), \
                            boost::bind(&utilSocket::set_result, this, &read_result, err));
    mpSocket->get_io_service().reset();
    qDebug() << "1a";
    mpSocket->get_io_service().run_one();

    while(mpSocket->get_io_service().run_one()) {
        if (timer_result) {
            mpSocket->cancel();
            break;
        }
        else if (read_result) {
           // boost::asio::async_read(*mpSocket, sb, boost::asio::transfer_exactly(mMsgLen), \
           //                         boost::bind(&utilSocket::set_result, this, &read_result, err));
        }

        cnt += 1;

    }


    if (err == boost::asio::error::eof) {
        this->disconnect();
        return false;
    }
    qDebug() << "2";
    boost::asio::streambuf::const_buffers_type bufs = sb.data();
    std::string retStr(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));
    boost::trim_right_if(retStr, boost::is_any_of(std::string("\0",1))); //  Remove null characters

    // Sanity check
    if (retStr.size() < mMsgLen) {
     return false;
    }

    message = retStr.substr(retStr.size()-mMsgLen, retStr.size()-1); // Pull most recent message
    qDebug() << "3";
    return true;
}
*/
