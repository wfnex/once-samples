/***********************************************************************
 * Copyright (C) 2013-2015, Nanjing WFNEX Network Technology Co., Ltd 
 * Description: 
 * Others:
 * Version:          V1.0
 * Author:           Yi Jian <yijian@wfnex.com>
 * Date:             2015-01-9
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#include "IDIPC.h"
#include "IONCEMain.h"
#include "CAWACEWrapper.h"
#include "ISYSLOG.h"

//using namespace ACEWrapperNS;
using namespace std;

class ProcessA : public IONCEProcessSink
    ,public IDIPCAcceptorConnectorSink
    ,public IDIPCTransportSink
    ,public CAWTimerWrapperIDSink
    
{
public:
    virtual void OnProcessRun()
    {
        SYSLOG_INFO("ProcessA::OnProcessRun");  
        IDIPCManager::Instance().CreateServer(m_acceptor);
        m_acceptor->StartListen(this,1);
    }
    virtual void OnConnectIndication(
        int aReason,
        IDIPCTransport *aTrpt,
        IDIPCAcceptorConnectorId *aRequestId)
    {
         SYSLOG_INFO("ProcessA::OnConnectIndication");  
         m_transport = aTrpt;
         m_transport->OpenWithSink(this);
         m_transport->SendData("i am processA",strlen("i am processA"));
         CAWTimeValue aInterval(2,0);
         m_timer.Schedule(this,aInterval,0);
    }
    virtual void OnTimer(CAWTimerWrapperID* aId)
    {
        for(int a=0;a<20;a++)
        {
            m_transport->SendData("i am processA",strlen("i am processA"));
        }
    }
    virtual void OnReceive(
        const char *rcvdata,
        size_t datasize,
        IDIPCTransport *aTrptId)
    {
        CAWString str(rcvdata,datasize);
        SYSLOG_INFO("ProcessA::OnReceive,str="<<str); 
        m_transport->SendData("i am processA",strlen("i am processA"));
    }

    virtual void OnDisconnect(
                int aReason,
                IDIPCTransport *aTrptId)
    {
        SYSLOG_INFO("ProcessA::OnDisconnect");  
    }
    virtual void OnSend(IDIPCTransport *aTrptId)
    {
        printf("ProcessA::OnSend");
    }
private:
    CAWAutoPtr < IDIPCAcceptor> m_acceptor;
    CAWAutoPtr < IDIPCTransport> m_transport;
    CAWTimerWrapperID m_timer;
};


int main(int argc, char** argv)
{
    ProcessA process;
    ONCEProcessRun(argc, argv, &process, 1000);
}



