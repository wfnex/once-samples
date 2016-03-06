#include "dipc.h"
#include "IONCEMain.h"
#include "CAWACEWrapper.h"
#include "ISYSLOG.h"
//using namespace ACEWrapperNS;

class ProcessB : public IDIPCProcessSink
    ,public IDIPCAcceptorConnectorSink
    ,public IDIPCTransportSink
    
{
public:
    virtual void OnProcessRun(int argc, char** argv, IDIPCProcess *dipcProcess)
    {
        SYSLOG_INFO("ProcessB::OnProcessRun");  
        dipcProcess->CreateClient(m_connector);
        m_connector->AsycConnect(this,1000,1);
    }
    void OnBackUpProcessConnected(CAWAutoPtr<IDIPCTransport> &transport)
    {
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
    virtual void OnReceive(CAWMessageBlock& msg,
        IDIPCTransport *aTrptId)
    {
        //CAWString str(rcvdata,datasize);
        //SYSLOG_INFO("ProcessB::OnReceive,str="<<str); 

        //m_transport->SendData("i am processB",strlen("i am processB"));
    }

    virtual void OnDisconnect(
                CAWResult aReason,
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



