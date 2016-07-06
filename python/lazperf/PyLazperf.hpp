#pragma once

#include <laz-perf/common/common.hpp>
#include <laz-perf/compressor.hpp>
#include <laz-perf/decompressor.hpp>

#include <laz-perf/encoder.hpp>
#include <laz-perf/decoder.hpp>
#include <laz-perf/formats.hpp>
#include <laz-perf/io.hpp>
#include <laz-perf/las.hpp>

#include <string>
#include <vector>

#include "PyLazperfTypes.hpp"


namespace pylazperf
{

template <typename CTYPE = unsigned char>
class TypedLazPerfBuf
{
    typedef std::vector<CTYPE> LazPerfRawBuf;

public:
    LazPerfRawBuf& m_buf;
    size_t m_idx;

    TypedLazPerfBuf(LazPerfRawBuf& buf) : m_buf(buf), m_idx(0)
    {}

    void putBytes(const unsigned char *b, size_t len)
    {
        m_buf.insert(m_buf.end(), (const CTYPE *)b, (const CTYPE *)(b + len));
    }
    void putByte(const unsigned char b)
        {   m_buf.push_back((CTYPE)b); }
    unsigned char getByte()
        { return (unsigned char)m_buf[m_idx++]; }
    void getBytes(unsigned char *b, int len)
    {
        memcpy(b, m_buf.data() + m_idx, len);
        m_idx += len;
    }
};


class LAZEngine
{
public:
    LAZEngine();
    virtual ~LAZEngine() {};
    virtual size_t getPointSize() const { return m_pointSize; }

protected:
    size_t m_pointSize;
};

class Decompressor : public LAZEngine {
public:
    Decompressor(std::vector<uint8_t>&);
    ~Decompressor(){};
    void add_dimension(pylazperf::Type t);
    size_t decompress(char* out, size_t buffer_size);


private:

    typedef laszip::decoders::arithmetic<TypedLazPerfBuf<uint8_t>> Decoder;
    typedef typename laszip::formats::dynamic_field_decompressor<Decoder>::ptr
        Engine;

    TypedLazPerfBuf<uint8_t> m_stream;
    Decoder m_decoder;
    Engine m_decompressor;
};


class Compressor : public LAZEngine
{
public:
    Compressor(std::vector<uint8_t>&);
    ~Compressor(){};
    void done();
    void add_dimension(pylazperf::Type t);
    size_t compress(const char* input, size_t buffer_size);
    const std::vector<uint8_t>* data() const;

private:

    typedef laszip::encoders::arithmetic<TypedLazPerfBuf<uint8_t>> Encoder;
    typedef typename laszip::formats::dynamic_field_compressor<Encoder>::ptr
            Engine;

    TypedLazPerfBuf<uint8_t> m_stream;
    Encoder m_encoder;
    Engine m_compressor;
    bool m_done;
};

}
