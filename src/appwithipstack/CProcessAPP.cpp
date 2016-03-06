#include "CProcessAPP.h"
#include "ISYSLOG.h"
#include "CMacAddress.h"

InitConfigDefaultParam params[]=
{
    {"ONCEAPP","test","2000", "test command"}, 
};

CProcessAPP::CProcessAPP()
    :m_ipif(NULL)
    ,m_dipcProcess(NULL)
{
}

CProcessAPP::~CProcessAPP()
{
}

//IWFNOSProcessSink
void CProcessAPP::OnProcessRun(int argc, char** argv, IDIPCProcess *dipcProcess)
{
    printf("CProcessAPP::OnProcessRun\n");
    m_dipcProcess = dipcProcess;
    IOAMManager::Instance().StartOAM(dipcProcess, this);

    IPacketService::Instance().Open(dipcProcess, this);
    dipcProcess->ProcessRunFinishNotify();
}

void CProcessAPP::OnBackUpProcessConnected(CAWAutoPtr<IDIPCTransport> &transport)
{
    printf("CProcessAPP::OnBackUpProcessConnected\n");
    m_backuptransport=transport;
}


//IOAMManagerSink
int CProcessAPP::OnOAMStart()
{
    int count = sizeof(params)/sizeof(InitConfigDefaultParam);
    printf("CProcessAPP::OnOAMStart, count=%d\n", count);

    return IOAMManager::Instance().InitCfg(params,count);
}

void CProcessAPP::ServiceStart()
{
    CAW_INFO_TRACE("CProcessAPP::OnProcessRun test info trace");
    CAW_WARNING_TRACE("CProcessAPP::OnProcessRun test info trace");
    CAW_ERROR_TRACE("CProcessAPP::OnProcessRun test info trace");

    printf ("CProcessAPP::ServiceStart\n");
}

void CProcessAPP::ServiceStop()
{
    printf("CProcessAPP::ServiceStop\n");
}

void CProcessAPP::ServiceRestart()
{
    printf("CProcessAPP::ServiceRestart\n");
}

void CProcessAPP::OnPortInitDone(uint8_t maxportid)
{
    printf("CProcessAPP::OnPortInitDone, maxportid=%d\n", maxportid);  
    char macaddr [ 6 ];
    IPacketService::Instance().GetPortMac(0,macaddr);
    CMacAddress addr;
    addr.SetMacAddr(macaddr);
    CAWString name = IPacketService::Instance().GetPortName(0);
    IPacketService::Instance().RequestPortPacket(0);

    m_ipif = CreateIPStack(this, "192.168.1.123",
        "255.255.255.0",
        "192.168.1.1",0, macaddr);
    printf("CProcessAPP::OnPortInitDone, macaddr=%s, devicename=%s\n", addr.ToString(), name.c_str()); 

    IPSConnectionManager::Instance();

#if 0

    if (ACE_Thread_Manager::instance ()->spawn(CProcessAPP::client_worker, this) == -1)
    {
        return;
    }

#endif

#if 0

    IPSConnectionManager::Instance()->CreateConnectionServer(IPSConnectionManager::CTYPE_TCP
        ,m_acceptor.ParaOut());

    CAWInetAddr aAddrPeer("192.168.137.100",8888);

    m_acceptor->StartListen(&m_serversink,aAddrPeer);

#endif




    CAWInetAddr aAddrPeer2("192.168.1.101",8443);
    //CAWInetAddr aAddrlocal("192.168.137.100",8443);

    //printf("start connect\n");
    IPSConnectionManager::Instance()->CreateConnectionClient(IPSConnectionManager::CTYPE_TCP
        ,m_connector.ParaOut());
    m_connector->AsycConnect(this,aAddrPeer2);
    //CAWTimeValue interval(2,0);
    //m_timer.Schedule(this,interval);

}

void CProcessAPP::OnPacket(const char *pkt, size_t pktsize, uint8_t portid)
{
    //printf("CProcessAPP::OnPacket, pktsize=%d, portid=%d\n", pktsize, portid);
    char macaddr [ 6 ];
    IPacketService::Instance().GetPortMac(portid,macaddr);
    CMacAddress addr;
    addr.SetMacAddr(macaddr);
    CAWString name = IPacketService::Instance().GetPortName(portid);
    //printf("CProcessAPP::OnPacket, macaddr=%s, devicename=%s\n", addr.ToString(), name.c_str());

    if (portid == 0)
    {
        if (m_ipif)
        {
            //printf("CProcessAPP::OnPacket, IPStackInput size=%d, portid=%d\n", pktsize, portid);
            m_ipif->IPStackInput(pkt,pktsize);
        }
    }
}

void CProcessAPP::OnConnectIndication(
    CAWResult aReason,
    IPSTransport *aTrpt,
    IPSAcceptorConnectorId *aRequestId)
{
    printf("CProcessAPP::OnConnectIndication, aReason=%d,%p\n", aReason, aTrpt);
    if ((aReason == CAW_OK) && (aTrpt != NULL))
    {
        CAWInetAddr aAddrPeer;
        CAWInetAddr aAddrLocal;
        aTrpt->GetLocalAddr(aAddrLocal);
        aTrpt->GetPeerAddr(aAddrPeer);

        CAW_INFO_TRACE("CProcessAPP::OnConnectIndication, localaddr="<<aAddrLocal.GetIpDisplayName()
            <<",peeraddr="<<aAddrPeer.GetIpDisplayName());

        
        m_pnewtrans = new CAPPTransport(aTrpt);
        aTrpt->OpenWithSink(m_pnewtrans);
        
        m_pnewtrans->RealMessageSend("hello",strlen("hello"));
    }
}
void CProcessAPP::OnTimer(CPSTimerWrapperID* aId)
{
    static int i=0;
    i++;
    if (i>10)
    {
        m_timer.Cancel();
    }
    printf("OnTimer\n");
}

void CProcessAPP::IPStackOutput(const char *pkt, size_t pktsize, uint8_t portid)
{
    printf("CCProcessAPP::IPStackOutput size=%d, portid=%d\n", pktsize, portid);
    IPacketService::Instance().SendPacket(pkt,pktsize,portid);
}


int CAPPTransport::RealMessageSend(const char *data,size_t datasize)
{
    CAWMessageBlock mbOn(
                datasize, 
                data, 
                CAWMessageBlock::DONT_DELETE, 
                datasize);

    if (m_transport->SendData(mbOn) != CAW_OK)
    {
        CAW_ERROR_TRACE("CAcceptorTestAPP::RealSendData");  
    }
    return 0;
}


CAcceptorTestAPP::CAcceptorTestAPP()
{
}

CAcceptorTestAPP::~CAcceptorTestAPP()
{
}

void CAcceptorTestAPP::OnConnectIndication(
    CAWResult aReason,
    IPSTransport *aTrpt,
    IPSAcceptorConnectorId *aRequestId)
{
    printf("CAcceptorTestAPP::OnConnectIndication, aReason=%d,%p\n", aReason, aTrpt);
    if (aTrpt)
    {
        m_transport = aTrpt;
        m_transport->OpenWithSink(this);
        CAWInetAddr aAddrPeer;
        CAWInetAddr aAddrLocal;
        aTrpt->GetLocalAddr(aAddrLocal);
        aTrpt->GetPeerAddr(aAddrPeer);

        CAW_INFO_TRACE("CAcceptorTestAPP::OnConnectIndication, localaddr="<<aAddrLocal.GetIpDisplayName()
            <<",peeraddr="<<aAddrPeer.GetIpDisplayName());

        RealMessageSend("hello",strlen("hello"));
        //m_transport->Disconnect(CAW_OK);
        //m_transport = NULL;
    }
}
int CAcceptorTestAPP::RealMessageSend(const char *data,size_t datasize)
{
    CAWMessageBlock mbOn(
                datasize, 
                data, 
                CAWMessageBlock::DONT_DELETE, 
                datasize);

    if (m_transport->SendData(mbOn) != CAW_OK)
    {
        CAW_ERROR_TRACE("CAcceptorTestAPP::RealSendData");  
    }
    return 0;
}


