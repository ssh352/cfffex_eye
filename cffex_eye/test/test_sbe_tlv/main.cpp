#include <vector>
#include <iostream>
#include <sys/time.h>
#include "tlv.h"
#include "Sbetest.h"

using namespace std;

struct my_type
{
    int param1;
    string param2;
    bool operator==(const my_type &r) const { return param1 == r.param1 && param2 == r.param2; }
};

struct test_msg_req
{
    int param1;
    int param2;
    int param3;
    int param4;
    double param5;
    string param6;
    string param7;
    string param8;
    my_type param9;
    vector<string> param10;
    bool operator==(const test_msg_req &r) const
    {
        return param1 == r.param1 && param2 == r.param2 && param3 == r.param3 && param4 == r.param4 &&
            param5 == r.param5 && param6 == r.param6 && param7 == r.param7 && param8 == r.param8 &&
            param9 == r.param9 && param10 == r.param10;
    }
};

void init_req(test_msg_req &req, bool all = true);
void tlv_encode(tlv_encoder &enc, test_msg_req &req, bool all = true);
void tlv_decode(const char *ptr, size_t len, test_msg_req &result, bool all = true);
void dump_buff(const char *ptr, size_t len);

size_t sbe_encode(char *buff, size_t len, test_msg_req &req);
void sbe_decode(char *buff, size_t len, test_msg_req &req);
long long get_time();

int main()
{
    const int count = 10000000;


    {

        test_msg_req req;
        init_req(req, false);

        cout << "==============tlv start===============" << endl;
        buffer buff;
        size_t t1 = get_time();
        for (int i = 0; i < count; ++i)
        {
            buff.reset();
            tlv_encoder enc(buff);
            tlv_encode(enc, req, false);
        }
        cout << "encode end, time: " << get_time() - t1 << endl;
        t1 = get_time();
        for (int i = 0; i < count; ++i)
        {
            test_msg_req req2;
            tlv_decode(buff.begin(), buff.length(), req2, false);
        }
        cout << "decode end, time: " << get_time() - t1 << endl;
        cout << "==============tlv end===============" << endl;
        
    }

    {
        test_msg_req req;
        init_req(req, false);
        char buff[4096];
        size_t len;
        cout << "==============sbe start===============" << endl;
        size_t t1 = get_time();
        for (int i = 0; i < count; ++i)
        {
            len = sbe_encode(buff, sizeof(buff), req);
        }
        cout << "encode end, time: " << get_time() - t1 << endl;
        t1 = get_time();
        for (int i = 0; i < count; ++i)
        {
            test_msg_req req2;
            sbe_decode(buff, len, req2);
        }
        cout << "decode end, time: " << get_time() - t1 << endl;
        cout << "==============sbe end===============" << endl;
    }
}

void test_tlv()
{
    test_msg_req req;
    init_req(req);
    buffer buff;
    tlv_encoder enc(buff);
    tlv_encode(enc, req);

    dump_buff(buff.begin(), buff.length());

    test_msg_req result;
    tlv_decode(buff.begin(), buff.length(), result);
    if (req == result)
    {
        cout << "ok" << endl;
    }
}

size_t sbe_encode(char *buff, size_t len, test_msg_req &req)
{
    sbetest::Sbetest s(buff, len);
    s.param1(req.param1).param2(req.param2).param3(req.param3)
        .param4(req.param4).param5(req.param5).putParam6(req.param6)
        .putParam7(req.param7).putParam8(req.param8);
    return s.encodedLength();
}

void sbe_decode(char *buff, size_t len, test_msg_req &req)
{
    sbetest::Sbetest s(buff, len);
    req.param1 = s.param1();
    req.param2 = s.param2();
    req.param3 = s.param3();
    req.param4 = s.param4();
    req.param5 = s.param5();
    req.param6 = s.getParam6AsString();
    req.param7 = s.getParam7AsString();
    req.param8 = s.getParam8AsString();
}

void tlv_encode(tlv_encoder &enc, test_msg_req &req, bool all)
{
    enc.set_value(1, req.param1);
    enc.set_value(2, req.param2);
    enc.set_value(3, req.param3);
    enc.set_value(4, req.param4);
    enc.set_value(5, req.param5);
    enc.set_value(6, req.param6);
    enc.set_value(7, req.param7);
    enc.set_value(8, req.param8);
    if (!all)return;
    tlv_encoder p9 = enc.add_tlv(9);
    p9.set_value(1, req.param9.param1);
    p9.set_value(2, req.param9.param2);
    p9.set_length();
    for (size_t i = 0; i < req.param10.size(); ++i)
    {
        enc.set_value(10, req.param10[i]);
    }
}

void tlv_decode(const char *ptr, size_t len, test_msg_req &result, bool all)
{
    tlv_decoder dec(ptr, len);
    dec.get_value(1, result.param1);
    dec.get_value(2, result.param2);
    dec.get_value(3, result.param3);
    dec.get_value(4, result.param4);
    dec.get_value(5, result.param5);
    dec.get_value(6, result.param6);
    dec.get_value(7, result.param7);
    dec.get_value(8, result.param8);
    if (!all)return;
    tlv_decoder p9;
    dec.get_value(9, p9);
    p9.get_value(1, result.param9.param1);
    p9.get_value(2, result.param9.param2);
    dec.get_value(10, result.param10);
}

void init_req(test_msg_req &req, bool all/* = true*/)
{
    req.param1 = 1;
    req.param2 = 2;
    req.param3 = 3;
    req.param4 = 4;
    req.param5 = 5.1234f;
    req.param6 = "param_6";
    req.param7 = "param_7";
    req.param8 = "param_8==================================================================================";
    if (!all) return;
    req.param9.param1 = 1;
    req.param9.param2 = "param_9.string";
    req.param10.push_back("hello world! i'm a tlv test");
    req.param10.push_back("ok");
}

void dump_buff(const char *ptr_, size_t len)
{
    const unsigned char *ptr = (const unsigned char *)ptr_;
    int left = len % 8;
    for (int i = 0; i < len / 8; ++i)
    {
        printf("[%02d]    %02x %02x %02x %02x    %02x %02x %02x %02x\n", 
            i + 1, *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5), *(ptr + 6), *(ptr + 7));
        ptr += 8;
    }

    if (left > 0)
    {
        printf("[%02d]    ", len / 8 + 1);
        for (int i = 0; i < left; ++i)
        {
            if (i == 4) printf("   ");
            printf("%02x ", *(ptr + i));
        }
        printf("\n");
    }
}

long long get_time()
{
    timeval ttime;
    gettimeofday(&ttime, 0);
    return ((long long)(ttime.tv_sec)) * 1000 + ttime.tv_usec / 1000;
}
