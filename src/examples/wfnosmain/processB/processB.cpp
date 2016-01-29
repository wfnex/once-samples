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

class ProcessB : public IONCEProcessSink
    ,public IDIPCAcceptorConnectorSink
    ,public IDIPCTransportSink
    
{
public:
    virtual void OnProcessRun()
    {
        SYSLOG_INFO("ProcessB::OnProcessRun");  
        IDIPCManager::Instance().CreateClient(m_connector);
        m_connector->AsycConnect(this,1000,1);
    }
    virtual void OnConnectIndication(
        int aReason,
        IDIPCTransport *aTrpt,
        IDIPCAcceptorConnectorId *aRequestId)
    {
         SYSLOG_INFO("ProcessB::OnConnectIndication"); 
         m_transport = aTrpt;
         aTrpt->OpenWithSink(this);
         //aTrpt->SendData("hello",strlen("hello"));
    }
    virtual void OnReceive(
        const char *rcvdata,
        size_t datasize,
        IDIPCTransport *aTrptId)
    {
        CAWString str(rcvdata,datasize);
        SYSLOG_INFO("ProcessB::OnReceive,str="<<str); 

        //m_transport->SendData("i am processB",strlen("i am processB"));
    }

    virtual void OnDisconnect(
                int aReason,
                IDIPCTransport *aTrptId)
    {
        SYSLOG_INFO("ProcessB::OnDisconnect");  
    }
    virtual void OnSend(IDIPCTransport *aTrptId)
    {
        printf("ProcessB::OnSend");
    }

private:
    CAWAutoPtr < IDIPCConnector > m_connector;
    CAWAutoPtr < IDIPCTransport> m_transport;
};

int main(int argc, char** argv)
{
    ProcessB process;
    ONCEProcessRun(argc, argv, &process, 1001);
}



