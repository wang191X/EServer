#ifndef CGAODATASTRUCT_H
#define CGAODATASTRUCT_H

struct SelfStudyHead
{
    SelfStudyHead()
        : flag(0xaabbccdd), dataLength(0)
    {}
    unsigned int flag;
    unsigned int dataLength;
};

#endif