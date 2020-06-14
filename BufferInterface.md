This document describes CBuffer interface used by several ROM system calls and
is based on testing of APIs.

```
class CMinBuffer {
            CMinBuffer();
    virtual ~CMinBuffer();
    virtual int Peek() = 0;
    virtual int Next(void) = 0;
    virtual int Skip(void) = 0;
    virtual int Get(void) = 0;
    virutal Size Getn(UByte* p, Size n) = 0;
    virtual int CopyOut(UByte* p, Size& n) = 0;
    virtual int Put(int dataByte) = 0;
    virtual Size Putn(const UByte* p, Size n) = 0;
    virtual int CopyIn(const UByte* p, Size& n) = 0;
    virtual void Reset(void) = 0;
    virtual Size GetSize(void) const = 0;
    virtual Boolean AtEOF(void) const = 0;
};

class CBuffer : public CMinBuffer {
protected:
            CBuffer();
    virtual ~CBuffer();
    virtual int Peek() = 0;
    virtual int Next(void) = 0;
    virtual int Skip(void) = 0;
    virtual int Get(void) = 0;
    virutal Size Getn(UByte* p, Size n) = 0;
    virtual int CopyOut(UByte* p, Size& n) = 0;
    virtual int Put(int dataByte) = 0;
    virtual Size Putn(const UByte* p, Size n) = 0;
    virtual int CopyIn(const UByte* p, Size& n) = 0;
    virtual void Reset(void) = 0;
    virtual Size GetSize(void) const = 0;
    virtual Boolean AtEOF(void) const = 0;
    virtual Long Hide(Long count, int dir) = 0;
    virtual Size Seek(Long off, int dir) = 0;
    virtual Size Position(void) const = 0;
};

class CBufferSegment : public CBuffer {
public:
    static CBufferSegment* New(void);
    void Delete();

    NewtonErr Init(Boolean deleteSegments = true);
    NewtonErr Init(void* data,
                   Size len,
                   Boolean freeBuffer = false,
                   Size validOff = 0,
                   Long validCount = -1);

private:
            CBufferSegment();
    virtual ~CBufferSegment();

public:
    virtual int Peek();
    virtual int Next(void);
    virtual int Skip(void);
    virtual int Get(void);
    virutal Size Getn(UByte* p, Size n);
    virtual int CopyOut(UByte* p, Size& n);
    virtual int Put(int dataByte);
    virtual Size Putn(const UByte* p, Size n);
    virtual int CopyIn(const UByte* p, Size& n);
    virtual void Reset(void);
    virtual Size GetSize(void) const;
    virtual Boolean AtEOF(void) const;
    virtual Long Hide(Long count, int dir);
    virtual Size Seek(Long off, int dir);
    virtual Size Position(void) const;
};

class CBufferList : public CBuffer {
public:
    static CBufferList* New(void);
    void Delete();

    NewtonErr Init(Boolean deleteSegments = true);
    NewtonErr Init(CList* bufList, Boolean deleteSegments = true);

private:
            CBufferList();
    virtual ~CBufferList();

public:
    virtual int Peek();
    virtual int Next(void);
    virtual int Skip(void);
    virtual int Get(void);
    virutal Size Getn(UByte* p, Size n);
    virtual int CopyOut(UByte* p, Size& n);
    virtual int Put(int dataByte);
    virtual Size Putn(const UByte* p, Size n);
    virtual int CopyIn(const UByte* p, Size& n);
    virtual void Reset(void);
            void ResetMark(void);
    virtual Size GetSize(void) const;
    virtual Boolean AtEOF(void) const;
    virtual Long Hide(Long count, int dir);
    virtual Size Seek(Long off, int dir);
    virtual Size Position(void) const;

    CBuffer*	At(ArrayIndex index);
    CBuffer*	First(void);
    CBuffer*	Last(void);

    NewtonErr	Insert(CBuffer* item);
    NewtonErr	InsertBefore(ArrayIndex index, CBuffer* item);
    NewtonErr	InsertAt(ArrayIndex index, CBuffer* item);
    NewtonErr	InsertFirst(CBuffer* item);
    NewtonErr	InsertLast(CBuffer* item);

    NewtonErr	Remove(CBuffer* item);
    NewtonErr	RemoveAt(ArrayIndex index);
    NewtonErr	RemoveFirst(void);
    NewtonErr	RemoveLast(void);
    NewtonErr	RemoveAll(void);

    NewtonErr	Delete(CBuffer* item);
    NewtonErr	DeleteAt(ArrayIndex index);
    NewtonErr	DeleteFirst(void);
    NewtonErr	DeleteLast(void);
    NewtonErr	DeleteAll(void);

    ArrayIndex		GetIndex(CBuffer* item);
};
```

```
CBufferSegment::Init(Size len)
```

Initialize the segment buffer. If the buffer already contained data, it is disposed.
Data is initialized with NewPtr.

```
CBufferSegment::Init(void* data, Size len, Boolean freeBufer = false, Size validOff = 0, Long validCount = -1)
```

Create a new segment buffer.
data: data for the segment (not copied)
len: length of the segment
freeBuffer: whether the data will be freed with DisposPtr on Delete or subsequent Init calls.

```
int CBufferSegment::Get()
```

Get a single byte, or return -1 if EOF was reached.
Advance the position.

```
Size CBufferSegment::Putn(const UByte* p, Size n)
```

Put several bytes, up to n.
If n is <= 0, return n, otherwise return the number of bytes copied and
increment the position accordingly.

```
int CBufferSegment::CopyIn(const UByte* p, Size& n)
```

Put several bytes.
If n is <= 0, return 0.
Eventually calls Putn via the virtual table.
Update n with the number of bytes that are yet to copy (n - number of written bytes).
Return -1 if all bytes were copied, 0 otherwise.

```
Size CBufferSegment::Getn(UByte* p, Size n)
```

Get several bytes, up to n.
If n is <= 0, return n, otherwise return the number of bytes copied and
increment the position accordingly.

```
int CBufferSegment::CopyOut(const UByte* p, Size& n)
```

Get several bytes.
If n is <= 0, return 0.
Eventually calls Getn via the virtual table.
Update n with the number of bytes that are yet to copy (n - number of written bytes).
Return -1 if all bytes were copied, 0 otherwise.

```
void CBufferSegment::Reset()
```

Reset the position to 0.

CBufferList has its own position, however it's losely coupled with the
position of added buffers.
