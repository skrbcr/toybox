#include <fstream>
#include <zlib.h>

namespace skrbcr {
bool ezin(std::string strFile, void* pData, unsigned long long sizeData) {
    std::ifstream ifs(strFile, std::ios::in | std::ios::binary);
    if (!ifs) return false;
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    if (inflateInit(&zs) != Z_OK) {
	    ifs.close();
	    return -1;
    }
    zs.avail_out = sizeData;
    zs.next_out = reinterpret_cast<Bytef*>(pData);
    unsigned char inbuf[32768];
    for (int k = 0; ; ++k) {
	    ifs.read(reinterpret_cast<char*>(inbuf), sizeof(inbuf));
	    zs.avail_in = ifs.gcount();
	    if (zs.avail_in == 0) break;
	    zs.next_in = inbuf;
	    if (inflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
	        inflateEnd(&zs);
	        ifs.close();
	        return false;
	    }
    }
    return true;
}

bool ezout(std::string strFile, void* pData, unsigned long long sizeData) {
    std::ofstream ofs(strFile, std::ios::out | std::ios::binary);
    if (!ofs) return false;
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
	    ofs.close();
	    return false;
    }
    zs.next_in = reinterpret_cast<Bytef*>(pData);
    zs.avail_in = sizeData;
    unsigned char outbuf[32768];
    do {
	    zs.next_out = outbuf;
	    zs.avail_out = sizeof(outbuf);
	    if (deflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
	        deflateEnd(&zs);
	        ofs.close();
	        return false;
	    }
	    ofs.write(reinterpret_cast<char*>(outbuf), sizeof(outbuf) - zs.avail_out);
    } while (zs.avail_out == 0);
    deflateEnd(&zs);
    ofs.close();
    return true;
}
}
