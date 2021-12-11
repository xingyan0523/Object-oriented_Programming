#include <iostream>
#include <map>
#include <queue>
#include <list>
#include <utility>
#include <climits>
#include <functional>
#include <iomanip>
#include <stack>
#include <cmath>
#include <vector>

using namespace std;

#define SET(func_name, type, var_name, _var_name) \
    void func_name(type _var_name) { var_name = _var_name; }
#define GET(func_name, type, var_name) \
    type func_name() const { return var_name; }

class header;
class payload;
class packet;
class node;
class event;
class link; // new

// for simplicity, we use a const int to simulate the delay
// if you want to simulate the more details, you should revise it to be a class
const unsigned int ONE_HOP_DELAY = 10;
const unsigned int BROCAST_ID = UINT_MAX;
unsigned int X_MAX, Y_MAX; //ccu 

// BROCAST_ID means that all neighbors are receivers; UINT_MAX is the maximum value of unsigned int

class header
{
public:
    virtual ~header() {}

    SET(setSrcID, unsigned int, srcID, _srcID);
    SET(setDstID, unsigned int, dstID, _dstID);
    SET(setPreID, unsigned int, preID, _preID);
    SET(setNexID, unsigned int, nexID, _nexID);
    GET(getSrcID, unsigned int, srcID);
    GET(getDstID, unsigned int, dstID);
    GET(getPreID, unsigned int, preID);
    GET(getNexID, unsigned int, nexID);

    virtual string type() = 0;

    // factory concept: generate a header
    class header_generator
    {
        // lock the copy constructor
        header_generator(header_generator &) {}
        // store all possible types of header
        static map<string, header_generator *> prototypes;

    protected:
        // allow derived class to use it
        header_generator() {}
        // after you create a new header type, please register the factory of this header type by this function
        void register_header_type(header_generator *h) { prototypes[h->type()] = h; }
        // you have to implement your own generate() to generate your header
        virtual header *generate() = 0;

    public:
        // you have to implement your own type() to return your header type
        virtual string type() = 0;
        // this function is used to generate any type of header derived
        static header *generate(string type)
        {
            if (prototypes.find(type) != prototypes.end())
            {                                        // if this type derived exists
                return prototypes[type]->generate(); // generate it!!
            }
            std::cerr << "no such header type" << std::endl; // otherwise
            return nullptr;
        }
        static void print()
        {
            cout << "registered header types: " << endl;
            for (map<string, header::header_generator *>::iterator it = prototypes.begin(); it != prototypes.end(); it++)
                cout << it->second->type() << endl;
        }
        virtual ~header_generator(){};
    };

protected:
    header() : srcID(BROCAST_ID), dstID(BROCAST_ID), preID(BROCAST_ID), nexID(BROCAST_ID) {} // this constructor cannot be directly called by users

private:
    unsigned int srcID;
    unsigned int dstID;
    unsigned int preID;
    unsigned int nexID;
    header(header &) {} // this constructor cannot be directly called by users
};
map<string, header::header_generator *> header::header_generator::prototypes;

class GR_header : public header
{
    double dstX; // the position of the destination
    double dstY;
    double srcX; // the position of the source
    double srcY;

    GR_header(GR_header &) {} // cannot be called by users

protected:
    GR_header() : dstX(0), dstY(0) {} // this constructor cannot be directly called by users

public:
    ~GR_header() {}

    SET(setDstX, double, dstX, _dstX);
    GET(getDstX, double, dstX);
    SET(setDstY, double, dstY, _dstY);
    GET(getDstY, double, dstY);
    SET(setSrcX, double, srcX, _srcX);
    GET(getSrcX, double, srcX);
    SET(setSrcY, double, srcY, _srcY);
    GET(getSrcY, double, srcY);

    string type() { return "GR_header"; }

    class GR_header_generator;
    friend class GR_header_generator;
    // GR_header_generator is derived from header_generator to generate a header
    class GR_header_generator : public header_generator
    {
        static GR_header_generator sample;
        // this constructor is only for sample to register this header type
        GR_header_generator()
        { /*cout << "GR_header registered" << endl;*/
            register_header_type(&sample);
        }

    protected:
        virtual header *generate()
        {
            // cout << "GR_header generated" << endl;
            return new GR_header;
        }

    public:
        virtual string type() { return "GR_header"; }
        ~GR_header_generator() {}
    };
};
GR_header::GR_header_generator GR_header::GR_header_generator::sample;

class HI_header : public header //ccu
{
    double dstX; // the position of the destination
    double dstY;
    double srcX; // the position of the source
    double srcY;

    HI_header(HI_header &) {} // cannot be called by users

protected:
    HI_header() : dstX(0), dstY(0) {} // this constructor cannot be directly called by users

public:
    ~HI_header() {}

    SET(setDstX, double, dstX, _dstX);
    GET(getDstX, double, dstX);
    SET(setDstY, double, dstY, _dstY);
    GET(getDstY, double, dstY);
    SET(setSrcX, double, srcX, _srcX);
    GET(getSrcX, double, srcX);
    SET(setSrcY, double, srcY, _srcY);
    GET(getSrcY, double, srcY);

    string type() { return "HI_header"; }

    class HI_header_generator;
    friend class HI_header_generator;
    // HI_header_generator is derived from header_generator to generate a header
    class HI_header_generator : public header_generator
    {
        static HI_header_generator sample;
        // this constructor is only for sample to register this header type
        HI_header_generator()
        { /*cout << "HI_header registered" << endl;*/
            register_header_type(&sample);
        }

    protected:
        virtual header *generate()
        {
            // cout << "HI_header generated" << endl;
            return new HI_header;
        }

    public:
        virtual string type() { return "HI_header"; }
        ~HI_header_generator() {}
    };
};
HI_header::HI_header_generator HI_header::HI_header_generator::sample;

class Rep_header : public header //ccu
{
    double dstX; // the position of the destination
    double dstY;
    double srcX; // the position of the source
    double srcY;

    Rep_header(Rep_header &) {} // cannot be called by users

protected:
    Rep_header() : dstX(0), dstY(0) {} // this constructor cannot be directly called by users

public:
    ~Rep_header() {}

    SET(setDstX, double, dstX, _dstX);
    GET(getDstX, double, dstX);
    SET(setDstY, double, dstY, _dstY);
    GET(getDstY, double, dstY);
    SET(setSrcX, double, srcX, _srcX);//ccu
    GET(getSrcX, double, srcX);
    SET(setSrcY, double, srcY, _srcY);
    GET(getSrcY, double, srcY);


    string type() { return "Rep_header"; }

    class Rep_header_generator;
    friend class Rep_header_generator;
    // Rep_header_generator is derived from header_generator to generate a header
    class Rep_header_generator : public header_generator
    {
        static Rep_header_generator sample;
        // this constructor is only for sample to register this header type
        Rep_header_generator()
        { /*cout << "Rep_header registered" << endl;*/
            register_header_type(&sample);
        }

    protected:
        virtual header *generate()
        {
            // cout << "Rep_header generated" << endl;
            return new Rep_header;
        }

    public:
        virtual string type() { return "Rep_header"; }
        ~Rep_header_generator() {}
    };
};
Rep_header::Rep_header_generator Rep_header::Rep_header_generator::sample;

class Ret_header : public header //ccu
{
    double dstX; // the position of the destination
    double dstY;
    double srcX; // the position of the source
    double srcY;
    unsigned int cacheID;

    Ret_header(Ret_header &) {} // cannot be called by users

protected:
    Ret_header() : dstX(0), dstY(0) {} // this constructor cannot be directly called by users

public:
    ~Ret_header() {}

    SET(setDstX, double, dstX, _dstX);
    GET(getDstX, double, dstX);
    SET(setDstY, double, dstY, _dstY);
    GET(getDstY, double, dstY);
    SET(setSrcX, double, srcX, _srcX);
    GET(getSrcX, double, srcX);
    SET(setSrcY, double, srcY, _srcY);
    GET(getSrcY, double, srcY);
    SET(setcacheID, unsigned int, cacheID, _cacheID);
    GET(getcacheID, unsigned int, cacheID);

    string type() { return "Ret_header"; }

    class Ret_header_generator;
    friend class Ret_header_generator;
    // Ret_header_generator is derived from header_generator to generate a header
    class Ret_header_generator : public header_generator
    {
        static Ret_header_generator sample;
        // this constructor is only for sample to register this header type
        Ret_header_generator()
        { /*cout << "Ret_header registered" << endl;*/
            register_header_type(&sample);
        }

    protected:
        virtual header *generate()
        {
            // cout << "Ret_header generated" << endl;
            return new Ret_header;
        }

    public:
        virtual string type() { return "Ret_header"; }
        ~Ret_header_generator() {}
    };
};
Ret_header::Ret_header_generator Ret_header::Ret_header_generator::sample;

class Res_header : public header //ccu
{
    double dstX; // the position of the destination
    double dstY;
    double srcX; // the position of the source
    double srcY;
    unsigned int cacheID;

    Res_header(Res_header &) {} // cannot be called by users

protected:
    Res_header() : dstX(0), dstY(0) {} // this constructor cannot be directly called by users

public:
    ~Res_header() {}

    SET(setDstX, double, dstX, _dstX);
    GET(getDstX, double, dstX);
    SET(setDstY, double, dstY, _dstY);
    GET(getDstY, double, dstY);
    SET(setSrcX, double, srcX, _srcX);
    GET(getSrcX, double, srcX);
    SET(setSrcY, double, srcY, _srcY);
    GET(getSrcY, double, srcY);
    SET(setcacheID, unsigned int, cacheID, _cacheID);
    GET(getcacheID, unsigned int, cacheID);

    string type() { return "Res_header"; }

    class Res_header_generator;
    friend class Res_header_generator;
    // Res_header_generator is derived from header_generator to generate a header
    class Res_header_generator : public header_generator
    {
        static Res_header_generator sample;
        // this constructor is only for sample to register this header type
        Res_header_generator()
        { /*cout << "Res_header registered" << endl;*/
            register_header_type(&sample);
        }

    protected:
        virtual header *generate()
        {
            // cout << "Res_header generated" << endl;
            return new Res_header;
        }

    public:
        virtual string type() { return "Res_header"; }
        ~Res_header_generator() {}
    };
};
Res_header::Res_header_generator Res_header::Res_header_generator::sample;

class payload
{
    payload(payload &) {} // this constructor cannot be directly called by users
protected:
    payload() {}

public:
    virtual ~payload() {}
    virtual string type() = 0;

    class payload_generator
    {
        // lock the copy constructor
        payload_generator(payload_generator &) {}
        // store all possible types of header
        static map<string, payload_generator *> prototypes;

    protected:
        // allow derived class to use it
        payload_generator() {}
        // after you create a new payload type, please register the factory of this payload type by this function
        void register_payload_type(payload_generator *h) { prototypes[h->type()] = h; }
        // you have to implement your own generate() to generate your payload
        virtual payload *generate() = 0;

    public:
        // you have to implement your own type() to return your header type
        virtual string type() = 0;
        // this function is used to generate any type of header derived
        static payload *generate(string type)
        {
            if (prototypes.find(type) != prototypes.end())
            {                                        // if this type derived exists
                return prototypes[type]->generate(); // generate it!!
            }
            std::cerr << "no such payload type" << std::endl; // otherwise
            return nullptr;
        }
        static void print()
        {
            cout << "registered payload types: " << endl;
            for (map<string, payload::payload_generator *>::iterator it = prototypes.begin(); it != prototypes.end(); it++)
                cout << it->second->type() << endl;
        }
        virtual ~payload_generator(){};
    };
};
map<string, payload::payload_generator *> payload::payload_generator::prototypes;

class GR_payload : public payload
{
    GR_payload(GR_payload &) {}

    string msg;

protected:
    GR_payload() {} // this constructor cannot be directly called by users
public:
    ~GR_payload() {}

    SET(setMsg, string, msg, _msg);
    GET(getMsg, string, msg);

    string type() { return "GR_payload"; }

    class GR_payload_generator;
    friend class GR_payload_generator;
    // GR_payload is derived from payload_generator to generate a payload
    class GR_payload_generator : public payload_generator
    {
        static GR_payload_generator sample;
        // this constructor is only for sample to register this payload type
        GR_payload_generator()
        { /*cout << "GR_payload registered" << endl;*/
            register_payload_type(&sample);
        }

    protected:
        virtual payload *generate()
        {
            // cout << "GR_payload generated" << endl;
            return new GR_payload;
        }

    public:
        virtual string type() { return "GR_payload"; }
        ~GR_payload_generator() {}
    };
};
GR_payload::GR_payload_generator GR_payload::GR_payload_generator::sample;

class HI_payload : public payload //ccu
{
    HI_payload(HI_payload &) {}

    string msg;

protected:
    HI_payload() {} // this constructor cannot be directly called by users
public:
    ~HI_payload() {}

    SET(setMsg, string, msg, _msg);
    GET(getMsg, string, msg);

    string type() { return "HI_payload"; }

    class HI_payload_generator;
    friend class HI_payload_generator;
    // HI_payload is derived from payload_generator to generate a payload
    class HI_payload_generator : public payload_generator
    {
        static HI_payload_generator sample;
        // this constructor is only for sample to register this payload type
        HI_payload_generator()
        { /*cout << "HI_payload registered" << endl;*/
            register_payload_type(&sample);
        }

    protected:
        virtual payload *generate()
        {
            // cout << "HI_payload generated" << endl;
            return new HI_payload;
        }

    public:
        virtual string type() { return "HI_payload"; }
        ~HI_payload_generator() {}
    };
};
HI_payload::HI_payload_generator HI_payload::HI_payload_generator::sample;

class Rep_payload : public payload //ccu
{
    Rep_payload(Rep_payload &) {}

    string msg;

protected:
    Rep_payload() {} // this constructor cannot be directly called by users
public:
    ~Rep_payload() {}

    SET(setMsg, string, msg, _msg);
    GET(getMsg, string, msg);

    string type() { return "Rep_payload"; }

    class Rep_payload_generator;
    friend class Rep_payload_generator;
    // Rep_payload is derived from payload_generator to generate a payload
    class Rep_payload_generator : public payload_generator
    {
        static Rep_payload_generator sample;
        // this constructor is only for sample to register this payload type
        Rep_payload_generator()
        { /*cout << "Rep_payload registered" << endl;*/
            register_payload_type(&sample);
        }

    protected:
        virtual payload *generate()
        {
            // cout << "Rep_payload generated" << endl;
            return new Rep_payload;
        }

    public:
        virtual string type() { return "Rep_payload"; }
        ~Rep_payload_generator() {}
    };
};
Rep_payload::Rep_payload_generator Rep_payload::Rep_payload_generator::sample;

class Ret_payload : public payload //ccu
{
    Ret_payload(Ret_payload &) {}

    string msg;

protected:
    Ret_payload() {} // this constructor cannot be directly called by users
public:
    ~Ret_payload() {}

    SET(setMsg, string, msg, _msg);
    GET(getMsg, string, msg);

    string type() { return "Ret_payload"; }

    class Ret_payload_generator;
    friend class Ret_payload_generator;
    // Ret_payload is derived from payload_generator to generate a payload
    class Ret_payload_generator : public payload_generator
    {
        static Ret_payload_generator sample;
        // this constructor is only for sample to register this payload type
        Ret_payload_generator()
        { /*cout << "Ret_payload registered" << endl;*/
            register_payload_type(&sample);
        }

    protected:
        virtual payload *generate()
        {
            // cout << "Ret_payload generated" << endl;
            return new Ret_payload;
        }

    public:
        virtual string type() { return "Ret_payload"; }
        ~Ret_payload_generator() {}
    };
};
Ret_payload::Ret_payload_generator Ret_payload::Ret_payload_generator::sample;

class Res_payload : public payload //ccu
{
    Res_payload(Res_payload &) {}

    string msg;

protected:
    Res_payload() {} // this constructor cannot be directly called by users
public:
    ~Res_payload() {}

    SET(setMsg, string, msg, _msg);
    GET(getMsg, string, msg);

    string type() { return "Res_payload"; }

    class Res_payload_generator;
    friend class Res_payload_generator;
    // Res_payload is derived from payload_generator to generate a payload
    class Res_payload_generator : public payload_generator
    {
        static Res_payload_generator sample;
        // this constructor is only for sample to register this payload type
        Res_payload_generator()
        { /*cout << "Res_payload registered" << endl;*/
            register_payload_type(&sample);
        }

    protected:
        virtual payload *generate()
        {
            // cout << "Res_payload generated" << endl;
            return new Res_payload;
        }

    public:
        virtual string type() { return "Res_payload"; }
        ~Res_payload_generator() {}
    };
};
Res_payload::Res_payload_generator Res_payload::Res_payload_generator::sample;

class packet
{
    // a packet usually contains a header and a payload
    header *hdr;
    payload *pld;
    unsigned int p_id;
    static unsigned int last_packet_id;

    packet(packet &) {}
    static int live_packet_num;

protected:
    // these constructors cannot be directly called by users
    packet() : hdr(nullptr), pld(nullptr)
    {
        p_id = last_packet_id++;
        live_packet_num++;
    }
    packet(string _hdr, string _pld, bool rep = false, unsigned int rep_id = 0)
    {
        if (!rep) // a duplicated packet does not have a new packet id
            p_id = last_packet_id++;
        else
            p_id = rep_id;
        hdr = header::header_generator::generate(_hdr);
        pld = payload::payload_generator::generate(_pld);
        live_packet_num++;
    }

public:
    virtual ~packet()
    {
        // cout << "packet destructor begin" << endl;
        if (hdr != nullptr)
            delete hdr;
        if (pld != nullptr)
            delete pld;
        live_packet_num--;
        // cout << "packet destructor end" << endl;
    }

    SET(setHeader, header *, hdr, _hdr);
    GET(getHeader, header *, hdr);
    SET(setPayload, payload *, pld, _pld);
    GET(getPayload, payload *, pld);
    GET(getPacketID, unsigned int, p_id);

    static void discard(packet *&p)
    {
        // cout << "checking" << endl;
        if (p != nullptr)
        {
            // cout << "discarding" << endl;
            // cout << p->type() << endl;
            delete p;
            // cout << "discarded" << endl;
        }
        p = nullptr;
        // cout << "checked" << endl;
    }
    virtual string type() = 0;

    static int getLivePacketNum() { return live_packet_num; }

    class packet_generator;
    friend class packet_generator;
    class packet_generator
    {
        // lock the copy constructor
        packet_generator(packet_generator &) {}
        // store all possible types of packet
        static map<string, packet_generator *> prototypes;

    protected:
        // allow derived class to use it
        packet_generator() {}
        // after you create a new packet type, please register the factory of this payload type by this function
        void register_packet_type(packet_generator *h) { prototypes[h->type()] = h; }
        // you have to implement your own generate() to generate your payload
        virtual packet *generate(packet *p = nullptr) = 0;

    public:
        // you have to implement your own type() to return your packet type
        virtual string type() = 0;
        // this function is used to generate any type of packet derived
        static packet *generate(string type)
        {
            if (prototypes.find(type) != prototypes.end())
            {                                        // if this type derived exists
                return prototypes[type]->generate(); // generate it!!
            }
            std::cerr << "no such packet type" << std::endl; // otherwise
            return nullptr;
        }
        static packet *replicate(packet *p)
        {
            if (prototypes.find(p->type()) != prototypes.end())
            {                                              // if this type derived exists
                return prototypes[p->type()]->generate(p); // generate it!!
            }
            std::cerr << "no such packet type" << std::endl; // otherwise
            return nullptr;
        }
        static void print()
        {
            cout << "registered packet types: " << endl;
            for (map<string, packet::packet_generator *>::iterator it = prototypes.begin(); it != prototypes.end(); it++)
                cout << it->second->type() << endl;
        }
        virtual ~packet_generator(){};
    };
};
map<string, packet::packet_generator *> packet::packet_generator::prototypes;
unsigned int packet::last_packet_id = 0;
int packet::live_packet_num = 0;

// this packet is used to tell the destination the msg
class GR_packet : public packet
{
    GR_packet(GR_packet &) {}

protected:
    GR_packet() {} // this constructor cannot be directly called by users
    GR_packet(packet *p) : packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID())
    {
        *(dynamic_cast<GR_header *>(this->getHeader())) = *(dynamic_cast<GR_header *>(p->getHeader()));
        *(dynamic_cast<GR_payload *>(this->getPayload())) = *(dynamic_cast<GR_payload *>(p->getPayload()));
        //DFS_path = (dynamic_cast<GR_header*>(p))->DFS_path;
        //isVisited = (dynamic_cast<GR_header*>(p))->isVisited;
    } // for duplicate
    GR_packet(string _h, string _p) : packet(_h, _p) {}

public:
    virtual ~GR_packet() {}
    string type() { return "GR_packet"; }

    class GR_packet_generator;
    friend class GR_packet_generator;
    // GR_packet is derived from packet_generator to generate a pub packet
    class GR_packet_generator : public packet_generator
    {
        static GR_packet_generator sample;
        // this constructor is only for sample to register this packet type
        GR_packet_generator()
        { /*cout << "GR_packet registered" << endl;*/
            register_packet_type(&sample);
        }

    protected:
        virtual packet *generate(packet *p = nullptr)
        {
            // cout << "GR_packet generated" << endl;
            if (nullptr == p)
                return new GR_packet("GR_header", "GR_payload");
            else
                return new GR_packet(p); // duplicate
        }

    public:
        virtual string type() { return "GR_packet"; }
        ~GR_packet_generator() {}
    };
};
GR_packet::GR_packet_generator GR_packet::GR_packet_generator::sample;

class HI_packet : public packet //ccu
{
    HI_packet(HI_packet &) {}

protected:
    HI_packet() {} // this constructor cannot be directly called by users
    HI_packet(packet *p) : packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID())
    {
        *(dynamic_cast<HI_header *>(this->getHeader())) = *(dynamic_cast<HI_header *>(p->getHeader()));
        *(dynamic_cast<HI_payload *>(this->getPayload())) = *(dynamic_cast<HI_payload *>(p->getPayload()));
        //DFS_path = (dynamic_cast<HI_header*>(p))->DFS_path;
        //isVisited = (dynamic_cast<HI_header*>(p))->isVisited;
    } // for duplicate
    HI_packet(string _h, string _p) : packet(_h, _p) {}

public:
    virtual ~HI_packet() {}
    string type() { return "HI_packet"; }

    class HI_packet_generator;
    friend class HI_packet_generator;
    // HI_packet is derived from packet_generator to generate a pub packet
    class HI_packet_generator : public packet_generator
    {
        static HI_packet_generator sample;
        // this constructor is only for sample to register this packet type
        HI_packet_generator()
        { /*cout << "HI_packet registered" << endl;*/
            register_packet_type(&sample);
        }

    protected:
        virtual packet *generate(packet *p = nullptr)
        {
            // cout << "HI_packet generated" << endl;
            if (nullptr == p)
                return new HI_packet("HI_header", "HI_payload");
            else
                return new HI_packet(p); // duplicate
        }

    public:
        virtual string type() { return "HI_packet"; }
        ~HI_packet_generator() {}
    };
};
HI_packet::HI_packet_generator HI_packet::HI_packet_generator::sample;

class Rep_packet : public packet //ccu
{
    Rep_packet(Rep_packet &) {}

protected:
    Rep_packet() {} // this constructor cannot be directly called by users
    Rep_packet(packet *p) : packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID())
    {
        *(dynamic_cast<Rep_header *>(this->getHeader())) = *(dynamic_cast<Rep_header *>(p->getHeader()));
        *(dynamic_cast<Rep_payload *>(this->getPayload())) = *(dynamic_cast<Rep_payload *>(p->getPayload()));
        //DFS_path = (dynamic_cast<Rep_header*>(p))->DFS_path;
        //isVisited = (dynamic_cast<Rep_header*>(p))->isVisited;
    } // for duplicate
    Rep_packet(string _h, string _p) : packet(_h, _p) {}

public:
    virtual ~Rep_packet() {}
    string type() { return "Rep_packet"; }

    class Rep_packet_generator;
    friend class Rep_packet_generator;
    // Rep_packet is derived from packet_generator to generate a pub packet
    class Rep_packet_generator : public packet_generator
    {
        static Rep_packet_generator sample;
        // this constructor is only for sample to register this packet type
        Rep_packet_generator()
        { /*cout << "Rep_packet registered" << endl;*/
            register_packet_type(&sample);
        }

    protected:
        virtual packet *generate(packet *p = nullptr)
        {
            // cout << "Rep_packet generated" << endl;
            if (nullptr == p)
                return new Rep_packet("Rep_header", "Rep_payload");
            else
                return new Rep_packet(p); // duplicate
        }

    public:
        virtual string type() { return "Rep_packet"; }
        ~Rep_packet_generator() {}
    };
};
Rep_packet::Rep_packet_generator Rep_packet::Rep_packet_generator::sample;

class Ret_packet : public packet //ccu
{
    Ret_packet(Ret_packet &) {}

protected:
    Ret_packet() {} // this constructor cannot be directly called by users
    Ret_packet(packet *p) : packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID())
    {
        *(dynamic_cast<Ret_header *>(this->getHeader())) = *(dynamic_cast<Ret_header *>(p->getHeader()));
        *(dynamic_cast<Ret_payload *>(this->getPayload())) = *(dynamic_cast<Ret_payload *>(p->getPayload()));
        //DFS_path = (dynamic_cast<Ret_header*>(p))->DFS_path;
        //isVisited = (dynamic_cast<Ret_header*>(p))->isVisited;
    } // for duplicate
    Ret_packet(string _h, string _p) : packet(_h, _p) {}

public:
    virtual ~Ret_packet() {}
    string type() { return "Ret_packet"; }

    class Ret_packet_generator;
    friend class Ret_packet_generator;
    // Ret_packet is derived from packet_generator to generate a pub packet
    class Ret_packet_generator : public packet_generator
    {
        static Ret_packet_generator sample;
        // this constructor is only for sample to register this packet type
        Ret_packet_generator()
        { /*cout << "Ret_packet registered" << endl;*/
            register_packet_type(&sample);
        }

    protected:
        virtual packet *generate(packet *p = nullptr)
        {
            // cout << "Ret_packet generated" << endl;
            if (nullptr == p)
                return new Ret_packet("Ret_header", "Ret_payload");
            else
                return new Ret_packet(p); // duplicate
        }

    public:
        virtual string type() { return "Ret_packet"; }
        ~Ret_packet_generator() {}
    };
};
Ret_packet::Ret_packet_generator Ret_packet::Ret_packet_generator::sample;

class Res_packet : public packet //ccu
{
    Res_packet(Res_packet &) {}

protected:
    Res_packet() {} // this constructor cannot be directly called by users
    Res_packet(packet *p) : packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID())
    {
        *(dynamic_cast<Res_header *>(this->getHeader())) = *(dynamic_cast<Res_header *>(p->getHeader()));
        *(dynamic_cast<Res_payload *>(this->getPayload())) = *(dynamic_cast<Res_payload *>(p->getPayload()));
        //DFS_path = (dynamic_cast<Res_header*>(p))->DFS_path;
        //isVisited = (dynamic_cast<Res_header*>(p))->isVisited;
    } // for duplicate
    Res_packet(string _h, string _p) : packet(_h, _p) {}

public:
    virtual ~Res_packet() {}
    string type() { return "Res_packet"; }

    class Res_packet_generator;
    friend class Res_packet_generator;
    // Res_packet is derived from packet_generator to generate a pub packet
    class Res_packet_generator : public packet_generator
    {
        static Res_packet_generator sample;
        // this constructor is only for sample to register this packet type
        Res_packet_generator()
        { /*cout << "Res_packet registered" << endl;*/
            register_packet_type(&sample);
        }

    protected:
        virtual packet *generate(packet *p = nullptr)
        {
            // cout << "Res_packet generated" << endl;
            if (nullptr == p)
                return new Res_packet("Res_header", "Res_payload");
            else
                return new Res_packet(p); // duplicate
        }

    public:
        virtual string type() { return "Res_packet"; }
        ~Res_packet_generator() {}
    };
};
Res_packet::Res_packet_generator Res_packet::Res_packet_generator::sample;

class node
{
    // all nodes created in the program
    static map<unsigned int, node *> id_node_table;

    unsigned int id;
    map<unsigned int, bool> phy_neighbors;

    // you can use the function to get the node's neighbors in HW2
    // But !!! In HW 3, you are not allowed to use this function
    // Please define your own neighbors in GR_node
    // Then, define your own Hello_packet to inherit packet
    // Broadcast Hello_packet to let your neighbors know your id
    const map<unsigned int, bool> &getPhyNeighbors()
    {
        return phy_neighbors;
    }

protected:
    node(node &) {} // this constructor should not be used
    node() {}       // this constructor should not be used
    node(unsigned int _id) : id(_id) { id_node_table[_id] = this; }

public:
    virtual ~node()
    { // erase the node
        id_node_table.erase(id);
    }

    void add_phy_neighbor(unsigned int _id, string link_type = "simple_link"); // we only add a directed link from id to _id
    void del_phy_neighbor(unsigned int _id);                                   // we only delete a directed link from id to _id

    void recv(packet *p)
    {
        packet *tp = p;
        recv_handler(tp);
        packet::discard(p);
    } // the packet will be directly deleted after the handler
    void send(packet *p);

    // receive the packet and do something; this is a pure virtual function
    virtual void recv_handler(packet *p) = 0;
    void send_handler(packet *P);

    static node *id_to_node(unsigned int _id) { return ((id_node_table.find(_id) != id_node_table.end()) ? id_node_table[_id] : nullptr); }
    GET(getNodeID, unsigned int, id);

    static void del_node(unsigned int _id)
    {
        if (id_node_table.find(_id) != id_node_table.end())
            id_node_table.erase(_id);
    }
    static unsigned int getNodeNum() { return id_node_table.size(); }

    class node_generator
    {
        // lock the copy constructor
        node_generator(node_generator &) {}
        // store all possible types of node
        static map<string, node_generator *> prototypes;

    protected:
        // allow derived class to use it
        node_generator() {}
        // after you create a new node type, please register the factory of this node type by this function
        void register_node_type(node_generator *h) { prototypes[h->type()] = h; }
        // you have to implement your own generate() to generate your node
        virtual node *generate(unsigned int _id) = 0;

    public:
        // you have to implement your own type() to return your node type
        virtual string type() = 0;
        // this function is used to generate any type of node derived
        static node *generate(string type, unsigned int _id)
        {
            if (id_node_table.find(_id) != id_node_table.end())
            {
                std::cerr << "duplicate node id" << std::endl; // node id is duplicated
                return nullptr;
            }
            else if (BROCAST_ID == _id)
            {
                cerr << "BROCAST_ID cannot be used" << endl;
                return nullptr;
            }
            else if (prototypes.find(type) != prototypes.end())
            { // if this type derived exists
                node *created_node = prototypes[type]->generate(_id);
                return created_node; // generate it!!
            }
            std::cerr << "no such node type" << std::endl; // otherwise
            return nullptr;
        }
        static void print()
        {
            cout << "registered node types: " << endl;
            for (map<string, node::node_generator *>::iterator it = prototypes.begin(); it != prototypes.end(); it++)
                cout << it->second->type() << endl;
        }
        virtual ~node_generator(){};
    };
};
map<string, node::node_generator *> node::node_generator::prototypes;
map<unsigned int, node *> node::id_node_table;

class mycomp
{
    bool reverse;

public:
    mycomp(const bool &revparam = false) { reverse = revparam; }
    bool operator()(const event *lhs, const event *rhs) const;
};

class event
{
    event(event *&) {} // this constructor cannot be directly called by users
    static priority_queue<event *, vector<event *>, mycomp> events;
    static unsigned int cur_time; // timer
    static unsigned int end_time;

    unsigned int trigger_time;

    // get the next event
    static event *get_next_event();
    static void add_event(event *e) { events.push(e); }
    static hash<string> event_seq;

protected:
    event() {} // it should not be used
    event(unsigned int _trigger_time) : trigger_time(_trigger_time) {}

public:
    virtual void trigger() = 0;
    virtual ~event() {}

    virtual unsigned int event_priority() const = 0;
    unsigned int get_hash_value(string string_for_hash) const
    {
        unsigned int priority = event_seq(string_for_hash);
        return priority;
    }

    static void flush_events(); // only for debug

    GET(getTriggerTime, unsigned int, trigger_time);

    static void start_simulate(unsigned int _end_time); // the function is used to start the simulation

    static unsigned int getCurTime() { return cur_time; }
    static void getCurTime(unsigned int _cur_time) { cur_time = _cur_time; }
    // static unsigned int getEndTime() { return end_time ; }
    // static void getEndTime(unsigned int _end_time) { end_time = _end_time; }

    virtual void print() const = 0; // the function is used to print the event information

    class event_generator
    {
        // lock the copy constructor
        event_generator(event_generator &) {}
        // store all possible types of event
        static map<string, event_generator *> prototypes;

    protected:
        // allow derived class to use it
        event_generator() {}
        // after you create a new event type, please register the factory of this event type by this function
        void register_event_type(event_generator *h) { prototypes[h->type()] = h; }
        // you have to implement your own generate() to generate your event
        virtual event *generate(unsigned int _trigger_time, void *data) = 0;

    public:
        // you have to implement your own type() to return your event type
        virtual string type() = 0;
        // this function is used to generate any type of event derived
        static event *generate(string type, unsigned int _trigger_time, void *data)
        {
            if (prototypes.find(type) != prototypes.end())
            { // if this type derived exists
                event *e = prototypes[type]->generate(_trigger_time, data);
                add_event(e);
                return e; // generate it!!
            }
            std::cerr << "no such event type" << std::endl; // otherwise
            return nullptr;
        }
        static void print()
        {
            cout << "registered event types: " << endl;
            for (map<string, event::event_generator *>::iterator it = prototypes.begin(); it != prototypes.end(); it++)
                cout << it->second->type() << endl;
        }
        virtual ~event_generator() {}
    };
};
map<string, event::event_generator *> event::event_generator::prototypes;
priority_queue<event *, vector<event *>, mycomp> event::events;
hash<string> event::event_seq;

unsigned int event::cur_time = 0;
unsigned int event::end_time = 0;

void event::flush_events()
{
    cout << "**flush begin" << endl;
    while (!events.empty())
    {
        cout << setw(11) << events.top()->trigger_time << ": " << setw(11) << events.top()->event_priority() << endl;
        delete events.top();
        events.pop();
    }
    cout << "**flush end" << endl;
}
event *event::get_next_event()
{
    if (events.empty())
        return nullptr;
    event *e = events.top();
    events.pop();
    // cout << events.size() << " events remains" << endl;
    return e;
}
void event::start_simulate(unsigned int _end_time)
{
    if (_end_time < 0)
    {
        cerr << "you should give a positive value of _end_time" << endl;
        return;
    }
    end_time = _end_time;
    event *e;
    e = event::get_next_event();
    while (e != nullptr && e->trigger_time <= end_time)
    {
        if (cur_time <= e->trigger_time)
            cur_time = e->trigger_time;
        else
        {
            cerr << "cur_time = " << cur_time << ", event trigger_time = " << e->trigger_time << endl;
            break;
        }

        // cout << "event trigger_time = " << e->trigger_time << endl;
        e->print(); // for log
        // cout << " event begin" << endl;
        e->trigger();
        // cout << " event end" << endl;
        delete e;
        e = event::get_next_event();
    }
    // cout << "no more event" << endl;
}

bool mycomp::operator()(const event *lhs, const event *rhs) const
{
    // cout << lhs->getTriggerTime() << ", " << rhs->getTriggerTime() << endl;
    // cout << lhs->type() << ", " << rhs->type() << endl;
    unsigned int lhs_pri = lhs->event_priority();
    unsigned int rhs_pri = rhs->event_priority();
    // cout << "lhs hash = " << lhs_pri << endl;
    // cout << "rhs hash = " << rhs_pri << endl;

    if (reverse)
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri < rhs_pri) : ((lhs->getTriggerTime()) < (rhs->getTriggerTime()));
    else
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri > rhs_pri) : ((lhs->getTriggerTime()) > (rhs->getTriggerTime()));
}

class recv_event : public event
{
public:
    class recv_data; // forward declaration

private:
    recv_event(recv_event &) {} // this constructor cannot be used
    recv_event() {}             // we don't allow users to new a recv_event by themselve
    unsigned int senderID;      // the sender
    unsigned int receiverID;    // the receiver
    packet *pkt;                // the packet

protected:
    // this constructor cannot be directly called by users; only by generator
    recv_event(unsigned int _trigger_time, void *data) : event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr)
    {
        recv_data *data_ptr = (recv_data *)data;
        senderID = data_ptr->s_id;
        receiverID = data_ptr->r_id;
        pkt = data_ptr->_pkt;
    }

public:
    virtual ~recv_event() {}
    // recv_event will trigger the recv function
    virtual void trigger();

    unsigned int event_priority() const;

    class recv_event_generator;
    friend class recv_event_generator;
    // recv_event is derived from event_generator to generate a event
    class recv_event_generator : public event_generator
    {
        static recv_event_generator sample;
        // this constructor is only for sample to register this event type
        recv_event_generator()
        { /*cout << "recv_event registered" << endl;*/
            register_event_type(&sample);
        }

    protected:
        virtual event *generate(unsigned int _trigger_time, void *data)
        {
            // cout << "recv_event generated" << endl;
            return new recv_event(_trigger_time, data);
        }

    public:
        virtual string type() { return "recv_event"; }
        ~recv_event_generator() {}
    };
    // this class is used to initialize the recv_event
    class recv_data
    {
    public:
        unsigned int s_id;
        unsigned int r_id;
        packet *_pkt;
    };

    void print() const;
};
recv_event::recv_event_generator recv_event::recv_event_generator::sample;

void recv_event::trigger()
{
    if (pkt == nullptr)
    {
        cerr << "recv_event error: no pkt!" << endl;
        return;
    }
    else if (node::id_to_node(receiverID) == nullptr)
    {
        cerr << "recv_event error: no node " << receiverID << "!" << endl;
        delete pkt;
        return;
    }
    node::id_to_node(receiverID)->recv(pkt);
}
unsigned int recv_event::event_priority() const
{
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string(receiverID) + to_string(pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the recv_event::print() function is used for log file
void recv_event::print() const
{
    cout << "time " << setw(11) << event::getCurTime()
         << "   recID " << setw(11) << receiverID
         << "   pktID" << setw(11) << pkt->getPacketID()
         << "   srcID " << setw(11) << pkt->getHeader()->getSrcID()
         << "   dstID" << setw(11) << pkt->getHeader()->getDstID()
         << "   preID" << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID" << setw(11) << pkt->getHeader()->getNexID()
         << endl;
}

class send_event : public event
{
public:
    class send_data; // forward declaration

private:
    send_event(send_event &) {}
    send_event() {} // we don't allow users to new a recv_event by themselves
    // this constructor cannot be directly called by users; only by generator
    unsigned int senderID;   // the sender
    unsigned int receiverID; // the receiver
    packet *pkt;             // the packet

protected:
    send_event(unsigned int _trigger_time, void *data) : event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr)
    {
        send_data *data_ptr = (send_data *)data;
        senderID = data_ptr->s_id;
        receiverID = data_ptr->r_id;
        pkt = data_ptr->_pkt;
    }

public:
    virtual ~send_event() {}
    // send_event will trigger the send function
    virtual void trigger();

    unsigned int event_priority() const;

    class send_event_generator;
    friend class send_event_generator;
    // send_event is derived from event_generator to generate a event
    class send_event_generator : public event_generator
    {
        static send_event_generator sample;
        // this constructor is only for sample to register this event type
        send_event_generator()
        { /*cout << "send_event registered" << endl;*/
            register_event_type(&sample);
        }

    protected:
        virtual event *generate(unsigned int _trigger_time, void *data)
        {
            // cout << "send_event generated" << endl;
            return new send_event(_trigger_time, data);
        }

    public:
        virtual string type() { return "send_event"; }
        ~send_event_generator() {}
    };
    // this class is used to initialize the send_event
    class send_data
    {
    public:
        unsigned int s_id;
        unsigned int r_id;
        packet *_pkt;
    };

    void print() const;
};
send_event::send_event_generator send_event::send_event_generator::sample;

void send_event::trigger()
{
    if (pkt == nullptr)
    {
        cerr << "send_event error: no pkt!" << endl;
        return;
    }
    else if (node::id_to_node(senderID) == nullptr)
    {
        cerr << "send_event error: no node " << senderID << "!" << endl;
        delete pkt;
        return;
    }
    node::id_to_node(senderID)->send(pkt);
}
unsigned int send_event::event_priority() const
{
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string(receiverID) + to_string(pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the send_event::print() function is used for log file
void send_event::print() const
{
    cout << "time " << setw(11) << event::getCurTime()
         << "   senID " << setw(11) << senderID
         << "   pktID" << setw(11) << pkt->getPacketID()
         << "   srcID " << setw(11) << pkt->getHeader()->getSrcID()
         << "   dstID" << setw(11) << pkt->getHeader()->getDstID()
         << "   preID" << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID" << setw(11) << pkt->getHeader()->getNexID()
         //<< "   type: " << setw(11) << pkt->type()
         //<< "   msg"         << setw(11) << dynamic_cast<GR_payload*>(pkt->getPayload())->getMsg()
         << endl;
}

class link
{
    // all links created in the program
    static map<pair<unsigned int, unsigned int>, link *> id_id_link_table;

    unsigned int id1; // from
    unsigned int id2; // to

protected:
    link(link &) {} // this constructor should not be used
    link() {}       // this constructor should not be used
    link(unsigned int _id1, unsigned int _id2) : id1(_id1), id2(_id2) { id_id_link_table[pair<unsigned int, unsigned int>(id1, id2)] = this; }

public:
    virtual ~link()
    {
        id_id_link_table.erase(pair<unsigned int, unsigned int>(id1, id2)); // erase the link
    }

    static link *id_id_to_link(unsigned int _id1, unsigned int _id2)
    {
        return ((id_id_link_table.find(pair<unsigned int, unsigned int>(_id1, _id2)) != id_id_link_table.end()) ? id_id_link_table[pair<unsigned, unsigned>(_id1, _id2)] : nullptr);
    }

    virtual double getLatency() = 0; // you must implement your own latency

    static void del_link(unsigned int _id1, unsigned int _id2)
    {
        pair<unsigned int, unsigned int> temp;
        if (id_id_link_table.find(temp) != id_id_link_table.end())
            id_id_link_table.erase(temp);
    }

    static unsigned int getLinkNum() { return id_id_link_table.size(); }

    class link_generator
    {
        // lock the copy constructor
        link_generator(link_generator &) {}
        // store all possible types of link
        static map<string, link_generator *> prototypes;

    protected:
        // allow derived class to use it
        link_generator() {}
        // after you create a new link type, please register the factory of this link type by this function
        void register_link_type(link_generator *h) { prototypes[h->type()] = h; }
        // you have to implement your own generate() to generate your link
        virtual link *generate(unsigned int _id1, unsigned int _id2) = 0;

    public:
        // you have to implement your own type() to return your link type
        virtual string type() = 0;
        // this function is used to generate any type of link derived
        static link *generate(string type, unsigned int _id1, unsigned int _id2)
        {
            if (id_id_link_table.find(pair<unsigned int, unsigned int>(_id1, _id2)) != id_id_link_table.end())
            {
                std::cerr << "duplicate link id" << std::endl; // link id is duplicated
                return nullptr;
            }
            else if (BROCAST_ID == _id1 || BROCAST_ID == _id2)
            {
                cerr << "BROCAST_ID cannot be used" << endl;
                return nullptr;
            }
            else if (prototypes.find(type) != prototypes.end())
            { // if this type derived exists
                link *created_link = prototypes[type]->generate(_id1, _id2);
                return created_link; // generate it!!
            }
            std::cerr << "no such link type" << std::endl; // otherwise
            return nullptr;
        }
        static void print()
        {
            cout << "registered link types: " << endl;
            for (map<string, link::link_generator *>::iterator it = prototypes.begin(); it != prototypes.end(); it++)
                cout << it->second->type() << endl;
        }
        virtual ~link_generator(){};
    };
};
map<string, link::link_generator *> link::link_generator::prototypes;
map<pair<unsigned int, unsigned int>, link *> link::id_id_link_table;

void node::add_phy_neighbor(unsigned int _id, string link_type)
{
    if (id == _id)
        return; // if the two nodes are the same...
    if (id_node_table.find(_id) == id_node_table.end())
        return; // if this node does not exist
    if (phy_neighbors.find(_id) != phy_neighbors.end())
        return; // if this neighbor has been added
    phy_neighbors[_id] = true;

    link::link_generator::generate(link_type, id, _id);
}
void node::del_phy_neighbor(unsigned int _id)
{
    phy_neighbors.erase(_id);
}

class simple_link : public link
{
protected:
    simple_link() {}                                                        // it should not be used outside the class
    simple_link(simple_link &) {}                                           // it should not be used
    simple_link(unsigned int _id1, unsigned int _id2) : link(_id1, _id2) {} // this constructor cannot be directly called by users

public:
    virtual ~simple_link() {}
    virtual double getLatency() { return ONE_HOP_DELAY; } // you must implement your own latency

    class simple_link_generator;
    friend class simple_link_generator;
    // simple_link is derived from link_generator to generate a link
    class simple_link_generator : public link_generator
    {
        static simple_link_generator sample;
        // this constructor is only for sample to register this link type
        simple_link_generator()
        { /*cout << "simple_link registered" << endl;*/
            register_link_type(&sample);
        }

    protected:
        virtual link *generate(unsigned int _id1, unsigned int _id2)
        { /*cout << "simple_link generated" << endl;*/
            return new simple_link(_id1, _id2);
        }

    public:
        virtual string type() { return "simple_link"; }
        ~simple_link_generator() {}
    };
};

simple_link::simple_link_generator simple_link::simple_link_generator::sample;

class GR_node : public node
{
    double x;
    double y;
    map<unsigned int, bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors
    map<unsigned int, pair<double, double>> coord_table;//ccu
    list<GR_packet*> GR_wait;
    bool hi; // this is used for example

protected:
    GR_node() {}                                        // it should not be used
    GR_node(GR_node &) {}                               // it should not be used
    GR_node(unsigned int _id) : node(_id), hi(false) {} // this constructor cannot be directly called by users

public:
    ~GR_node() {}

    SET(setX, double, x, _x);
    SET(setY, double, y, _y);
    GET(getX, double, x);
    GET(getY, double, y);

    // please define recv_handler function to deal with the incoming packet
    virtual void recv_handler(packet *p);

    void add_one_hop_neighbor(unsigned int n_id) { one_hop_neighbors[n_id] = true; }
    unsigned int get_one_hop_neighbor_num() { return one_hop_neighbors.size(); }
    void add_coord_table(unsigned int n_id, double x, double y) { coord_table[n_id].first = x; coord_table[n_id].second = y; }
    unsigned int get_coord_table_num() { return coord_table.size(); }//ccu
    
    class GR_node_generator;
    friend class GR_node_generator;
    // GR_node is derived from node_generator to generate a node
    class GR_node_generator : public node_generator
    {
        static GR_node_generator sample;
        // this constructor is only for sample to register this node type
        GR_node_generator()
        { /*cout << "GR_node registered" << endl;*/
            register_node_type(&sample);
        }

    protected:
        virtual node *generate(unsigned int _id)
        { /*cout << "GR_node generated" << endl;*/
            return new GR_node(_id);
        }

    public:
        virtual string type() { return "GR_node"; }
        ~GR_node_generator() {}
    };
};
GR_node::GR_node_generator GR_node::GR_node_generator::sample;

pair<unsigned int, unsigned int> myHash(unsigned int id){//ccu
    pair<unsigned int, unsigned int> c;
    unsigned int v1, v2;
    hash<string> coord;
    v1 = coord(to_string(id));
    v2 = coord(to_string(v1));
    c.first = (v1 % X_MAX) / 10000;
    c.second = (v2 % Y_MAX) / 10000;
    //cout<< c.first << "  " << c.second <<endl;
    return c;
}

// the function is used to add an initial event
void add_initial_event(unsigned int src, unsigned int dst, unsigned t = 0, string msg = "default")
{
    if (node::id_to_node(src) == nullptr || (dst != BROCAST_ID && node::id_to_node(dst) == nullptr))
    {
        cerr << "src or dst is incorrect" << endl;
        return;
    }

    if (msg == "hello") //ccu
    {
        HI_packet *pkt = dynamic_cast<HI_packet *>(packet::packet_generator::generate("HI_packet"));
        if (pkt == nullptr)
        {
            cerr << "packet type is incorrect" << endl;
            return;
        }
        HI_header *hdr = dynamic_cast<HI_header *>(pkt->getHeader());
        HI_payload *pld = dynamic_cast<HI_payload *>(pkt->getPayload());

        if (hdr == nullptr)
        {
            cerr << "header type is incorrect" << endl;
            return;
        }
        if (pld == nullptr)
        {
            cerr << "payload type is incorrect" << endl;
            return;
        }

        /*GR_node *dst_n = nullptr;

        if (dst != BROCAST_ID)
        {
            dst_n = dynamic_cast<GR_node *>(node::id_to_node(dst));
            if (dst_n == nullptr)
            {
                cerr << "node type is incorrect" << endl;
                return;
            }
            hdr->setDstX(dst_n->getX());
            hdr->setDstY(dst_n->getY());
        }*/

        hdr->setSrcID(src);
        hdr->setDstID(BROCAST_ID);
        hdr->setPreID(src);
        hdr->setNexID(BROCAST_ID);

        pld->setMsg(msg);

        recv_event::recv_data e_data;
        e_data.s_id = src;
        e_data.r_id = src;
        e_data._pkt = pkt;

        // for (unsigned int i = 0; i < 10; i ++){
        //     hdr->push_visited_node(i);
        // }
        // for (unsigned int i = 0; i < 5; i ++){
        //     hdr->mark_visited_node(i);
        // }

        recv_event *e = dynamic_cast<recv_event *>(event::event_generator::generate("recv_event", t, (void *)&e_data));
        if (e == nullptr)
            cerr << "event type is incorrect" << endl;
    }
    else if (msg == "publish") //ccu
    {
        Rep_packet *pkt = dynamic_cast<Rep_packet *>(packet::packet_generator::generate("Rep_packet"));
        if (pkt == nullptr)
        {
            cerr << "packet type is incorrect" << endl;
            return;
        }
        Rep_header *hdr = dynamic_cast<Rep_header *>(pkt->getHeader());
        Rep_payload *pld = dynamic_cast<Rep_payload *>(pkt->getPayload());

        if (hdr == nullptr)
        {
            cerr << "header type is incorrect" << endl;
            return;
        }
        if (pld == nullptr)
        {
            cerr << "payload type is incorrect" << endl;
            return;
        }

        hdr->setSrcID(src);
        hdr->setDstID(dst);
        hdr->setPreID(src);
        hdr->setNexID(dst);

        pld->setMsg(msg);

        recv_event::recv_data e_data;
        e_data.s_id = src;
        e_data.r_id = src;
        e_data._pkt = pkt;

        // for (unsigned int i = 0; i < 10; i ++){
        //     hdr->push_visited_node(i);
        // }
        // for (unsigned int i = 0; i < 5; i ++){
        //     hdr->mark_visited_node(i);
        // }

        recv_event *e = dynamic_cast<recv_event *>(event::event_generator::generate("recv_event", t, (void *)&e_data));
        if (e == nullptr)
            cerr << "event type is incorrect" << endl;
    }
    else
    {
        GR_packet *pkt = dynamic_cast<GR_packet *>(packet::packet_generator::generate("GR_packet"));
        if (pkt == nullptr)
        {
            cerr << "packet type is incorrect" << endl;
            return;
        }
        GR_header *hdr = dynamic_cast<GR_header *>(pkt->getHeader());
        GR_payload *pld = dynamic_cast<GR_payload *>(pkt->getPayload());

        if (hdr == nullptr)
        {
            cerr << "header type is incorrect" << endl;
            return;
        }
        if (pld == nullptr)
        {
            cerr << "payload type is incorrect" << endl;
            return;
        }

        hdr->setSrcID(src);
        hdr->setDstID(dst);
        hdr->setPreID(src);
        hdr->setNexID(src);

        pld->setMsg(msg);

        recv_event::recv_data e_data;
        e_data.s_id = src;
        e_data.r_id = src;
        e_data._pkt = pkt;

        // for (unsigned int i = 0; i < 10; i ++){
        //     hdr->push_visited_node(i);
        // }
        // for (unsigned int i = 0; i < 5; i ++){
        //     hdr->mark_visited_node(i);
        // }

        recv_event *e = dynamic_cast<recv_event *>(event::event_generator::generate("recv_event", t, (void *)&e_data));
        if (e == nullptr)
            cerr << "event type is incorrect" << endl;
    }
}

void setNodePos(unsigned int id, pair<double, double> pos)
{
    if (id == BROCAST_ID)
    {
        cerr << "node id is incorrect" << endl;
        return;
    }
    GR_node *ptr = dynamic_cast<GR_node *>(node::id_to_node(id));
    if (ptr == nullptr)
    {
        cerr << "node type is incorrect" << endl;
        return;
    }
    ptr->setX(pos.first);
    ptr->setY(pos.second);
}
pair<double, double> getNodePos(unsigned int id)
{
    if (id == BROCAST_ID)
    {
        cerr << "node id is incorrect" << endl;
        return pair<double, double>(0, 0);
    }
    GR_node *ptr = dynamic_cast<GR_node *>(node::id_to_node(id));
    if (ptr == nullptr)
    {
        cerr << "node type is incorrect" << endl;
        return pair<double, double>(0, 0);
    }
    return pair<double, double>(ptr->getX(), ptr->getY());
}

// send_handler function is used to transmit packet p based on the information in the header
// Note that the packet p will not be discard after send_handler ()

void node::send_handler(packet *p)
{
    packet *_p = packet::packet_generator::replicate(p);
    send_event::send_data e_data;
    e_data.s_id = _p->getHeader()->getPreID();
    e_data.r_id = _p->getHeader()->getNexID();
    e_data._pkt = _p;
    send_event *e = dynamic_cast<send_event *>(event::event_generator::generate("send_event", event::getCurTime(), (void *)&e_data));
    if (e == nullptr)
        cerr << "event type is incorrect" << endl;
}

void node::send(packet *p)
{ // this function is called by event; not for the user
    if (p == nullptr)
        return;

    unsigned int _nexID = p->getHeader()->getNexID();
    for (map<unsigned int, bool>::iterator it = phy_neighbors.begin(); it != phy_neighbors.end(); it++)
    {
        unsigned int nb_id = it->first; // neighbor id

        if (nb_id != _nexID && BROCAST_ID != _nexID)
            continue; // this neighbor will not receive the packet
        
        unsigned int trigger_time = event::getCurTime() + link::id_id_to_link(id, nb_id)->getLatency(); // we simply assume that the delay is fixed
        //cout << "node " << id << " send to node " << nb_id << " " << p->type() << endl;
        recv_event::recv_data e_data;
        e_data.s_id = id;
        e_data.r_id = nb_id;

        packet *p2 = packet::packet_generator::replicate(p);
        e_data._pkt = p2;

        recv_event *e = dynamic_cast<recv_event *>(event::event_generator::generate("recv_event", trigger_time, (void *)&e_data)); // send the packet to the neighbor
        if (e == nullptr)
            cerr << "event type is incorrect" << endl;
    }
    packet::discard(p);
}

double dst(unsigned int a, unsigned int b) {
    pair<double, double> coordinate1;
    pair<double, double> coordinate2;
    coordinate1 = getNodePos(a);
    coordinate2 = getNodePos(b);
    return sqrt(pow((coordinate1.first - coordinate2.first), 2) + pow((coordinate1.second - coordinate2.second), 2));
}

double dst(unsigned int a, double x, double y) {
    pair<double, double> coordinate1;
    coordinate1 = getNodePos(a);
    return sqrt(pow((coordinate1.first - x), 2) + pow((coordinate1.second - y), 2));
}

// you have to write the code in recv_handler
void GR_node::recv_handler(packet *p) //ccu
{
    unsigned int CUR = getNodeID();
    unsigned int SRC = p->getHeader()->getSrcID();
    unsigned int DST = p->getHeader()->getDstID();
    unsigned int PRE = p->getHeader()->getPreID();
    unsigned int NEXT = CUR;
    
    map<unsigned int, pair<double, double>>::const_iterator it;//table
    map<unsigned int, bool>::const_iterator iter;//nb

    if (p->type() == "GR_packet"){
        GR_packet *GR_pkt = dynamic_cast<GR_packet *>(p);
        GR_payload *GR_pld = dynamic_cast<GR_payload*>(p->getPayload());
        GR_header *GR_hdr = dynamic_cast<GR_header*>(p->getHeader());

        double dst_X, dst_Y;

        if(SRC == CUR){
            GR_hdr->setSrcX(getNodePos(CUR).first);
            GR_hdr->setSrcY(getNodePos(CUR).second);
        }
        
        it = coord_table.find(DST);
        iter = one_hop_neighbors.find(DST);
     
        if(iter != one_hop_neighbors.end()){//dst為neighbors
            //cout<<"state neb :"<<DST<<endl;
            GR_pld->setMsg("ok");//ok代表header的座標有設定過
            GR_hdr->setDstX(getNodePos(DST).first);
            GR_hdr->setDstY(getNodePos(DST).second);
            add_coord_table(DST, getNodePos(DST).first, getNodePos(DST).second);
        }
        else if(it != coord_table.end()){//dst在table裡
            //cout<<"state table :"<<DST<<endl;
            GR_pld->setMsg("ok");
            GR_hdr->setDstX(it->second.first);
            GR_hdr->setDstY(it->second.second);
        }
        else if(GR_pld->getMsg() == "default"){//找不到dst，送出Ret_packet

            //cout<<"state send ret"<<DST<<" "<<GR_pld->getMsg()<<endl;
            GR_packet *cache = dynamic_cast<GR_packet*>(packet::packet_generator::replicate(p));
            GR_wait.push_back(cache);//複製並暫存

            Ret_packet *RET_pkt = dynamic_cast<Ret_packet *> (packet::packet_generator::generate("Ret_packet"));
            Ret_header *RET_hdr = dynamic_cast<Ret_header *>(RET_pkt->getHeader());
            Ret_payload *RET_pld = dynamic_cast<Ret_payload *>(RET_pkt->getPayload());
            
            pair<unsigned int, unsigned int> hash; //ccu
            hash = myHash(DST);

            RET_hdr->setDstX(hash.first);
            RET_hdr->setDstY(hash.second);
            RET_hdr->setSrcX(GR_hdr->getSrcX());
            RET_hdr->setSrcY(GR_hdr->getSrcY());
            RET_hdr->setcacheID(GR_pkt->getPacketID());//紀錄暫存封包的ID

            double min = dst(CUR, hash.first, hash.second);

            for (iter = one_hop_neighbors.begin(); iter != one_hop_neighbors.end(); iter++){
                if (iter->second && dst(iter->first, hash.first, hash.second) < min){
                    min = dst(iter->first, hash.first, hash.second);
                    NEXT = iter->first;
                }
            }
            
            RET_hdr->setSrcID(CUR);
            RET_hdr->setDstID(BROCAST_ID);
            RET_hdr->setPreID(CUR);
            RET_hdr->setNexID(NEXT);

            RET_pld->setMsg(to_string(DST));

            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(NEXT != CUR) send_handler(RET_pkt);
            
            
            packet *del = static_cast<packet*> (RET_pkt);
            packet::discard(del);
            return;
        }
        
        dst_X = GR_hdr->getDstX();
        dst_Y = GR_hdr->getDstY();
        
        double min = dst(CUR, dst_X, dst_Y);

        for (iter = one_hop_neighbors.begin(); iter != one_hop_neighbors.end(); iter++){
            if (iter->second && dst(iter->first, dst_X, dst_Y) < min){
                min = dst(iter->first, dst_X, dst_Y);
                NEXT = iter->first;
            }
        }
        
        GR_pkt->getHeader()->setPreID(CUR);
        GR_pkt->getHeader()->setNexID(NEXT);
        
        if (NEXT != CUR) send_handler(GR_pkt);
    }
    else if (p->type() == "HI_packet"){
        HI_packet *HI_pkt = dynamic_cast<HI_packet *>(p);
        HI_header *HI_hdr = dynamic_cast<HI_header *>(p->getHeader());
        //cout << "node " << getNodeID() << " send the HI_packet" << endl;
        if (SRC == CUR){
            send_handler(HI_pkt);
        }
        else{
            add_one_hop_neighbor(HI_hdr->getSrcID());
        }
    }
    else if(p->type() == "Rep_packet"){        
        Rep_packet *REP_pkt = dynamic_cast<Rep_packet *>(p);
        Rep_header *REP_hdr = dynamic_cast<Rep_header *>(REP_pkt->getHeader());

        if(SRC == CUR){
            //find dead end
            pair<unsigned int, unsigned int> hash; //ccu
            hash = myHash(CUR);
            REP_hdr->setDstX(hash.first);
            REP_hdr->setDstY(hash.second);
            REP_hdr->setSrcX(getNodePos(CUR).first);
            REP_hdr->setSrcY(getNodePos(CUR).second);
        }
        double dst_X = REP_hdr->getDstX();
        double dst_Y = REP_hdr->getDstY();

        double min = dst(CUR, dst_X, dst_Y);

        for (iter = one_hop_neighbors.begin(); iter != one_hop_neighbors.end(); iter++){
            if (iter->second && dst(iter->first, dst_X, dst_Y) < min){
                min = dst(iter->first, dst_X, dst_Y);
                NEXT = iter->first;
            }
        }
        REP_hdr->setPreID(CUR);
        REP_hdr->setNexID(NEXT);
        
        //cout << "node " << getNodeID() << " send the Rep_packet" << endl;//debug
            
        if (NEXT != CUR) send_handler(REP_pkt); 
        else add_coord_table(SRC, REP_hdr->getSrcX(), REP_hdr->getSrcY());
        
    }
    else if(p->type() == "Ret_packet"){
        Ret_packet *RET_pkt = dynamic_cast<Ret_packet *>(p);
        Ret_header *RET_hdr = dynamic_cast<Ret_header *>(RET_pkt->getHeader());
        Ret_payload *RET_pld = dynamic_cast<Ret_payload *>(RET_pkt->getPayload());

        double dst_X = RET_hdr->getDstX();
        double dst_Y = RET_hdr->getDstY();
        unsigned int GR_dst = stoi(RET_pld->getMsg());
  
        double min = dst(CUR, dst_X, dst_Y);
        for (iter = one_hop_neighbors.begin(); iter != one_hop_neighbors.end(); iter++){
            if (iter->second && dst(iter->first, dst_X, dst_Y) < min){
                min = dst(iter->first, dst_X, dst_Y);
                NEXT = iter->first;
            }
        }
              
        if (NEXT != CUR){
            RET_hdr->setPreID(CUR);
            RET_hdr->setNexID(NEXT); 
            send_handler(RET_pkt);
        }
        else{
            it = coord_table.find(GR_dst);
            if(it != coord_table.end()){//如果table有dst資料，產生res並傳回dst
                Res_packet *RES_pkt = dynamic_cast<Res_packet *> (packet::packet_generator::generate("Res_packet"));
                Res_header *RES_hdr = dynamic_cast<Res_header *>(RES_pkt->getHeader());
                Res_payload *RES_pld = dynamic_cast<Res_payload *>(RES_pkt->getPayload());

                RES_hdr->setDstX(it->second.first);
                RES_hdr->setDstY(it->second.second);
                RES_hdr->setSrcX(RET_hdr->getSrcX());
                RES_hdr->setSrcY(RET_hdr->getSrcY());
                
                RES_hdr->setSrcID(CUR);
                RES_hdr->setDstID(SRC);
                RES_hdr->setPreID(CUR);
                RES_hdr->setNexID(PRE);
                RES_hdr->setcacheID(RET_hdr->getcacheID());

                RES_pld->setMsg(RET_pld->getMsg());
                
                send_handler(RES_pkt);
                packet *del_pkt = static_cast<packet*> (RES_pkt);
                packet::discard(del_pkt);
                return;
            }
            
        }
    }
    else if(p->type() == "Res_packet"){
        Res_packet *RES_pkt = dynamic_cast<Res_packet *>(p);
        Res_header *RES_hdr = dynamic_cast<Res_header*> (RES_pkt->getHeader());
        
        double src_X = RES_hdr->getSrcX();
        double src_Y = RES_hdr->getSrcY();
        //cout << "node " << getNodeID() << " send the Res_packet" << NEXT <<endl;//debug
             
        //*************greedy routing******************
        double min = dst(CUR, src_X, src_Y);
        for (iter = one_hop_neighbors.begin(); iter != one_hop_neighbors.end(); iter++){
            if (iter->second && dst(iter->first, src_X, src_Y) < min){
                min = dst(iter->first, src_X, src_Y);
                NEXT = iter->first;
            }
        }
        //*********************************************
        RES_hdr->setPreID(CUR);
        RES_hdr->setNexID(NEXT);

        if (DST != CUR) send_handler(RES_pkt);
        else{
            if(!GR_wait.empty()){
                list<GR_packet*>::iterator GR_it;
                for(GR_it = GR_wait.begin(); GR_it != GR_wait.end(); GR_it++)
                    if((*GR_it)->getPacketID() == RES_hdr->getcacheID())
                        break;

                GR_packet *GR_pkt = (*GR_it);
                GR_header *GR_hdr = dynamic_cast<GR_header*> (GR_pkt->getHeader());
                GR_payload *GR_pld = dynamic_cast<GR_payload*> (GR_pkt->getPayload());

                GR_pld->setMsg("re");
                GR_hdr->setDstX(RES_hdr->getDstX());
                GR_hdr->setDstY(RES_hdr->getDstY());

                double min = dst(CUR, RES_hdr->getDstX(), RES_hdr->getDstY());
                NEXT = CUR;
                for (iter = one_hop_neighbors.begin(); iter != one_hop_neighbors.end(); iter++){
                    if (iter->second && dst(iter->first, RES_hdr->getDstX(), RES_hdr->getDstY()) < min){
                        min = dst(iter->first, RES_hdr->getDstX(), RES_hdr->getDstY());
                        NEXT = iter->first;
                    }
                }
                GR_hdr->setPreID(CUR);
                GR_hdr->setNexID(NEXT);
                send_handler(GR_pkt);
                
                packet *del_pkt = static_cast<packet*> (GR_pkt);
                packet::discard(del_pkt);
            }
        }
    }

    
    // you should implement the GR distributed algorithm in recv_hander
    // getNodeID() returns the id of the current node
    //
    // The current node's neighbors are already stored in the following variable
    // map<unsigned int,bool> node::phy_neighbors
    // however, this variable is private in the class node
    // You have to use node::getPhyNeighbors to get the variable
    // for example, if you want to print all the neighbors of this node
    // const map<unsigned int,bool> &nblist = getPhyNeighbors();
    // for (map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
    //     cout << it->first << endl;
    // }
    //
    // you can use p->getHeader()->setSrcID() or getSrcID()
    //             p->getHeader()->setDstID() or getDstID()
    //             p->getHeader()->setPreID() or getPreID()
    //             p->getHeader()->setNexID() or getNexID() to change or read the packet header
    //
    // In addition, you can get the packet, header, and payload with the correct type (GR)
    // in fact, this is downcasting
    // GR_packet * pkt = dynamic_cast<GR_packet*> (p);
    // GR_header * hdr = dynamic_cast<GR_header*> (p->getHeader());
    // GR_payload * pld = dynamic_cast<GR_payload*> (p->getPayload());
    //
    // you can also change the GR_header setting
    // hdr->setDstX(double): to set the destination's position
    // hdr->setDstY(double)
    //
    // you can also change the GR_payload setting
    // pld->setMsg(string): to set the message transmitted to the destination
    //
    // Besides, you can use packet::packet_generator::generate() to generate a new packet; note that you should fill the header and payload in the packet
    // moreover, you can use "packet *p2 = packet::packet_generator::replicate(p)" to make a clone p2 of packet p
    // note that if the packet is generated or replicated manually, you must delete it by packet::discard() manually before recv_hander finishes
    //
    // "IMPORTANT":
    // You have to "carefully" fill the correct information (e.g., srcID, dstID, ...) in the packet before you send it
    // Note that if you want to transmit a packet to only one next node (i.e., unicast), then you fill the ID of a specific node to "nexID" in the header
    // Otherwise, i.e., you want to broadcasts, then you fill "BROCAST_ID" to "nexID" in the header
    // after that, you can use send() to transmit the packet
    // usage: send_handler (p);
    //
    // note that packet p will be discarded (deleted) after recv_hander(); you don't need to manually delete it
}

int main() //ccu
{
    // header::header_generator::print(); // print all registered headers
    // payload::payload_generator::print(); // print all registered payloads
    // packet::packet_generator::print(); // print all registered packets
    // node::node_generator::print(); // print all registered nodes
    // event::event_generator::print(); // print all registered events
    // link::link_generator::print(); // print all registered links

    unsigned int nodeNum;
    cin >> nodeNum >> X_MAX >> Y_MAX;

    for (unsigned int id = 0; id < nodeNum; id++){
        node::node_generator::generate("GR_node", id);
    }
    unsigned int id, BR_time, Rep_time;
    double x, y;
    pair<double, double> coordinate;
    for (unsigned int i = 0; i < nodeNum; i++){
        cin >> id >> x >> y >> BR_time >> Rep_time;
        add_initial_event(id, BROCAST_ID, BR_time, "hello");
        add_initial_event(id, BROCAST_ID, Rep_time, "publish");
        coordinate = make_pair(x, y);
        setNodePos(i, coordinate);
        for (unsigned int j = 0; j < i; j++){
            if (dst(i, j) <= 1.){
                node::id_to_node(i)->add_phy_neighbor(j);
                node::id_to_node(j)->add_phy_neighbor(i);
            }
        }
    }

    unsigned int pairs, time;
    cin >> pairs >> time;

    for (unsigned int round = 0; round < pairs; round++){
        unsigned int t, src, dst;
        cin >> t >> src >> dst;
        add_initial_event(src, dst, t);
    }

    // start simulation!!
    //event::start_simulate(time);
    event::start_simulate(time);

    //  for(int i = 0; i < nodeNum; i++){
    //      GR_node *n = dynamic_cast<GR_node*> (node::id_to_node(i));
    //      cout<<i<<" "<<n->get_one_hop_neighbor_num()<<" "<<endl;
    //  }
    //  cout<<endl;

    //event::flush_events() ;
    //cout << packet::getLivePacketNum() << endl;
    return 0;
}