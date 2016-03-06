#include "dipc.h"
#include "IONCEMain.h"
#include "CAWACEWrapper.h"
#include "ISYSLOG.h"

//using namespace ACEWrapperNS;
using namespace std;

class ProcessA : public IDIPCProcessSink
    ,public IDIPCAcceptorConnectorSink
    ,public IDIPCTransportSink
    ,public CAWTimerWrapperIDSink
    
{
public:
    virtual void OnProcessRun(int argc, char** argv, IDIPCProcess *dipcProcess)
    {
        SYSLOG_INFO("ProcessA::OnProcessRun");  
        dipcProcess->CreateServer(m_acceptor);
        m_acceptor->StartListen(this,1);
    }
    void OnBackUpProcessConnected(CAWAutoPtr<IDIPCTransport> &transport)
    {
        
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
    virtual void OnReceive(CAWMessageBlock& msg,
        IDIPCTransport *aTrptId)
    {
        //CAWString str(rcvdata,datasize);
        //SYSLOG_INFO("ProcessA::OnReceive,str="<<str); 
       // m_transport->SendData("i am processA",strlen("i am processA"));
    }

    virtual void OnDisconnect(
                CAWResult aReason,
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



