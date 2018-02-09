
#ifndef PAGE_RW_C
#define PAGE_RW_C

//#include "MyDB_PageReaderWriter.h"

#include "../../DatabaseTable/headers/MyDB_PageReaderWriter.h"
#include "../../DatabaseTable/headers/MyDB_PageRecIterator.h"

void MyDB_PageReaderWriter :: clear () {
	*((size_t *)((char*)this->myPage->getBytes() + sizeof(MyDB_PageType))) = sizeof(MyDB_PageType) + sizeof(size_t);// initialize the offset size
	*((MyDB_PageType*) this->myPage->getBytes()) = MyDB_PageType :: RegularPage;// initialize the pageType
	this->myPage->wroteBytes();
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	return *((MyDB_PageType*) this->myPage->getBytes());
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr iterateIntoMe) {
	return make_shared <MyDB_PageRecIterator> (*this, this->myPage, iterateIntoMe);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType toMe) {
	*((MyDB_PageType*) this->myPage->getBytes()) = toMe;
	this->myPage->wroteBytes();
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr appendMe) {
	size_t recSize = appendMe->getBinarySize();
	char* ptr = (char*)this->myPage->getBytes();
	if (*((size_t *)(ptr + sizeof(MyDB_PageType))) + recSize > this->pageSize)// page - offset = rest size
		return false;
	appendMe->toBinary(ptr+*((size_t *)(ptr+ sizeof(MyDB_PageType))));
    *((size_t *)(ptr+ sizeof(MyDB_PageType))) += recSize; // update offset
	this->myPage->wroteBytes();
	return true;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter (MyDB_TablePtr myTable, MyDB_BufferManagerPtr myBuffer, int whichPage) {
	this->myPage = myBuffer->getPage(myTable, whichPage);
	this->pageSize = myBuffer->getPageSize();
}

#endif
