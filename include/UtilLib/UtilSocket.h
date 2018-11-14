/***
Based off of: http://theboostcpplibraries.com/boost.asio-network-programming#ex.asio_05

https://gist.github.com/bechu/2423333

http://stackoverflow.com/questions/22581315/how-to-discard-data-as-it-is-sent-with-boostasio/22598329#22598329
***/


#ifndef UTILSOCKET_H
#define UTILSOCKET_H
#include "stdafx.h"
// Std
#include <string>

// Boost
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/optional.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/assign/list_of.hpp>


	// Qt
	//#include <QDebug>

	/*!
		@brief Client TCP Socket class

		This socket class uses boost sockets to send and receive messages. Messages are restricted
		to a pre-set message length.
	*/
	class UTIL_API utilSocket
	{
	public:
		/*!
			@brief utilSocket constructor
			@param messageLength Length of message
		 */
		utilSocket(int messageLength);

		/*!
			@brief utilSocket destructor
		*/
		~utilSocket();

		/*!
		 * \brief recvMessage Read data from socket
		 *
		 * \param message Message read from socket, with length mMsgLen
		 * \return Returns false if connection has been lost
		 */
		bool recvMessage(std::string& message); // Returns false if connection has been lost
		/*!
		 * \brief sendMessage
		 * \param message
		 * \return Returns false if connection has been lost
		 */
		bool sendMessage(std::string& message); // Returns false if connection has been lost

		/*!
		 * \brief Receive most recent message with timeout.
		 *
		 * This message will always wait timeout_sec before returning, and will return the
		 * most recent socket message, throwing away all old messages.
		 *
		 * \param message Return message
		 * \param timeout_msec Time to wait before returning
		 *
		 * \return bool Returns true if successful
		 */
		bool recvMessageTimeout(std::string& message, double timeout_msec);

		/*!
		 * \brief Connect to server
		 * \return bool Returns true if connected
		 */
		bool connect();

		/*!
		 * \brief Disconnect from server
		 */
		void disconnect();

		/*!
		 * \brief Set address of port/ip to connect to
		 * \param IP address (ex. "127.0.0.1" for same machine)
		 * \param Port number (ex. "12345")
		 */
		void setAddress(std::string ipaddr, std::string port);
	private:
		std::string _ip_addr; /*!< Socket IP address */
		std::string _port; /*!< Socket port */

		boost::asio::io_service _io_service; /*!< Boost I/O service*/
		boost::asio::ip::tcp::socket* _pSocket; /*!< Internal socket object */
		int _msg_len; /*!< Socket message length */

		/*!
		 * @brief Used to reset boost error code a to b
		 * @param a boost system error code
		 * @param b boost system error code
		 */
		void _setResult(boost::optional<boost::system::error_code> *a, boost::system::error_code b);
		/*!
		 * @brief Deletes all data currently in buffer
		 * @return Returns true if successful
		 */
		bool _flushBuffer();

	};


#endif // UTILSOCKET_H
