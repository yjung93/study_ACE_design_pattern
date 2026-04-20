/*
 * OutputHandler.hpp
 *
 *  Created on: Jan 26, 2025
 *      Author: yjung93
 */

#ifndef APPLICATIONS_EXAMPLE_ACCEPTOR_CONNECTOR_OutputHandler_HPP_
#define APPLICATIONS_EXAMPLE_ACCEPTOR_CONNECTOR_OutputHandler_HPP_

#include "framework/acceptor_connector/1_0/ServiceHandler.hpp"
#include "framework/acceptor_connector/1_0/SockStream.hpp"

namespace ex_acceptor_connector
{

class OutputHandler: public AcceptorConnector_1_0::ServiceHandler<AcceptorConnector_1_0::SockStream>
{
public:
    OutputHandler();
    virtual ~OutputHandler();

    int close();
    void sendMessage( string message );

private:
    int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
    int handleOutput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
};

} /* namespace ex_acceptor_connector */

#endif /* APPLICATIONS_EXAMPLE_ACCEPTOR_CONNECTOR_OutputHandler_HPP_ */
