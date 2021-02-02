#ifndef _PIPELINE_HPP_
#define _PIPELINE_HPP_

namespace zia
{

namespace system
{

    enum class PipelineStep {
        NewConnection,
        ConnectionFilter,
        NewMessage,
        NewMessageFilter,
        ProtocolDecode,
        ProtocolFilter,
        ProtocolHandle,
        NewReply,
        Replied,
    };

}

}    // namespace zia

#endif    //_PIPELINE_HPP_
