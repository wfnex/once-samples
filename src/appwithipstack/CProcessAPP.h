#ifndef CAPP_PROCESS_H
#define CAPP_PROCESS_H
#include "IOAM.h"
#include "dipc.h"
#include "IONCEMain.h"
#include "IPacketService.h"
#include "IPSConnectionInterface.h"
#include "IPSTimerWrapperID.h"
#include "IIPStack.h"

class CAPPTransport : public IPSTransportSink
{
public:
    CAPPTransport(IPSTransport *ptrans)
    {
        printf("CAPPTransport\n");
        m_transport = ptrans;
    };
    virtual ~CAPPTransport()
    {
        
    }
    virtual void OnReceive(
        CAWMessageBlock &aData,
        IPSTransport *aTrptId,
        CPSTransportParameter *aPara = NULL)
    {
        printf("OnReceive, %s\n", aData.GetTopLevelReadPtr());
        m_transport->Disconnect(0);
        m_transport = NULL;
    }

    virtual void OnSend(
        IPSTransport *aTrptId,
        CPSTransportParameter *aPara = NULL)
    {
         printf("OnSend");
    }

    virtual void OnDisconnect(
    CAWResult aReason,
    IPSTransport *aTrptId)
    {
        printf("OnDisconnect");
    }
    int RealMessageSend(const char *data,size_t datasize);
private:
    CAWAutoPtr<IPSTransport> m_transport;
};

class CAcceptorTestAPP : public IPSAcceptorConnectorSink,IPSTransportSink
{
public:
    CAcceptorTestAPP();
    virtual ~CAcceptorTestAPP();
    virtual void OnConnectIndication(
        CAWResult aReason,
        IPSTransport *aTrpt,
        IPSAcceptorConnectorId *aRequestId);

    virtual void OnReceive(
        CAWMessageBlock &aData,
        IPSTransport *aTrptId,
        CPSTransportParameter *aPara = NULL)
    {
        printf("OnReceive\n");
        m_transport->SendData(aData);
    }

    virtual void OnSend(
        IPSTransport *aTrptId,
        CPSTransportParameter *aPara = NULL)
    {
         printf("OnSend\n");
    }

    virtual void OnDisconnect(
    CAWResult aReason,
    IPSTransport *aTrptId)
    {
        printf("OnDisconnect\n");
        m_transport->Disconnect(CAW_OK);
        m_transport = NULL;
    }
    int RealMessageSend(const char *data,size_t datasize);

    
CAWAutoPtr<IPSTransport> m_transport;

};


class CProcessAPP : public IDIPCProcessSink 
                     , public IOAMManagerSink
                     , public IPacketServiceProcess
                     , public IPSAcceptorConnectorSink
                     , public IPSTimerWrapperIDSink
                     , public IIPStackSend
{
public:
    CProcessAPP();
    virtual ~CProcessAPP();
    
    //IWFNOSProcessSink
    virtual void OnProcessRun(int argc, char** argv, IDIPCProcess *dipcProcess);
    virtual void OnBackUpProcessConnected(CAWAutoPtr<IDIPCTransport> &transport);


    //IOAMManagerSink
    virtual int OnOAMStart();
    virtual void ServiceStart();
    virtual void ServiceStop();
    virtual void ServiceRestart();

    virtual void OnPortInitDone(uint8_t maxportid);
    virtual void OnPacket(const char *pkt, size_t pktsize, uint8_t portid);
    //static ACE_THR_FUNC_RETURN server_worker (void *p);
    //static ACE_THR_FUNC_RETURN client_worker (void *p);
    virtual void OnConnectIndication(
        CAWResult aReason,
        IPSTransport *aTrpt,
        IPSAcceptorConnectorId *aRequestId);
    virtual void OnTimer(CPSTimerWrapperID* aId);

    virtual void IPStackOutput(const char *pkt, size_t pktsize, uint8_t portid);
private:
    IIPStack *m_ipif;
    CAPPTransport *m_pnewtrans;
    CAWAutoPtr<IPSConnector> m_connector;
    CAWAutoPtr<IPSAcceptor> m_acceptor;
    CAcceptorTestAPP m_serversink;
    CPSTimerWrapperID m_timer;
    IDIPCProcess *m_dipcProcess;
    CAWAutoPtr<IDIPCTransport> m_backuptransport;
};

#endif // CIPOE_PROCESS_H

