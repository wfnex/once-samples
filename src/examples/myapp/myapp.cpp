#include "dipc.h"
#include "IONCEMain.h"
#include "CAWACEWrapper.h"

//using namespace ACEWrapperNS;
using namespace std;

class CMyApp : public IDIPCProcessSink
    ,public IDIPCAcceptorConnectorSink
    ,public IDIPCTransportSink
    ,public CAWTimerWrapperIDSink
    
{
public:
    virtual void OnProcessRun(int argc, char** argv, IDIPCProcess *dipcProcess)
    {
        printf("CMyApp::OnProcessRun");  
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
         printf("CMyApp::OnConnectIndication");  
         m_transport = aTrpt;
         m_transport->OpenWithSink(this);
         m_transport->SendData("i am CMyApp",strlen("i am CMyApp"));
         CAWTimeValue aInterval(2,0);
         m_timer.Schedule(this,aInterval,0);
    }
    virtual void OnTimer(CAWTimerWrapperID* aId)
    {
        for(int a=0;a<20;a++)
        {
            m_transport->SendData("i am CMyApp",strlen("i am CMyApp"));
        }
    }
    virtual void OnReceive(CAWMessageBlock &msg,
        IDIPCTransport *aTrptId)
    {
        //std::string str=std::string(rcvdata,datasize);
        //printf("CMyApp::OnReceive,str="); 
        //m_transport->SendData("i am CMyApp",strlen("i am CMyApp"));
    }

    virtual void OnDisconnect(
                CAWResult aReason,
                IDIPCTransport *aTrptId)
    {
        printf("CMyApp::OnDisconnect");  
    }
    virtual void OnSend(IDIPCTransport *aTrptId)
    {
        printf("CMyApp::OnSend");
    }
private:
    CAWAutoPtr < IDIPCAcceptor> m_acceptor;
    CAWAutoPtr < IDIPCTransport> m_transport;
    CAWTimerWrapperID m_timer;
};


int main(int argc, char** argv)
{
    CAW_INFO_TRACE("ssss");
    CAWString ss("issssss");
    CAW_INFO_TRACE("ssdfdf:"<<ss);
    CMyApp process;
    ONCEProcessRun(argc, argv, &process, 1000);
}