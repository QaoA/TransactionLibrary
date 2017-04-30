#ifndef __WRITE_TRANSACTION_H__
#define __WRITE_TRANSACTION_H__

#include "CLTransaction.h"
#include "CLWriteTransactionOpenedOjects.h"
#include "CLLogItemsSet.h"
#include "TransactionLibraryNameSpace.h"
#include "CLNameAddressPairSet.h"

TRANSACTIONLIB_NS_BEGIN

struct SLUserObjectInfo;
struct SLNVMNameAddressPair;

class CLWriteTransaction : public CLTransaction
{
public:
	CLWriteTransaction();
	virtual ~CLWriteTransaction();

private:
	virtual void Initialize() override;
	virtual void Uninitialize() override;
	virtual void OnCommit() override;
	virtual void OnAbort(CLTransactionAbort &) override;

public:
	CLTransactionalObject * OpenObjectRead(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);
	CLTransactionalObject * OpenObjectWrite(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);
	void ConvertOpenModeReadToWrite(CLTransactionalObject * pObject);
	bool SetName(char * strName, void * address);
	void * FindByName(char * strName);
	
private:
	SLNVMNameAddressPair * FindNameAddressPairByName(char * strName);
	
private:
	CLLogItemsSet m_itemSet;
	CLWriteTransactionOpenedOjects m_objectSet;
	CLNameAddressPairSet m_nameAddressPairSet;
};

TRANSACTIONLIB_NS_END
#endif