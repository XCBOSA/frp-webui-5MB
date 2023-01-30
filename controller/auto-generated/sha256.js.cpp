//
// Created by xcbosa on 2023-01-31
//

#include "../../processor/processor.h"
#include "../../utils/utils.h"
#include "../../webuiconf.h"

using namespace std;
using namespace xc::processor;
using namespace xc::utils;
using namespace xc::processor::templates;

namespace xc::controller {

    static string ControllerPath = "/sha256.js";

    static ResponseData *controllerResponse(RequestData request) {
        return new TextResponseData(200,
            string("/*\n") + 
            string("* A JavaScript implementation of the SHA256 hash function.\n") + 
            string("*\n") + 
            string("* FILE:	sha256.js\n") + 
            string("* VERSION:	0.8\n") + 
            string("* AUTHOR:	Christoph Bichlmeier <informatik@zombiearena.de>\n") + 
            string("*\n") + 
            string("* NOTE: This version is not tested thoroughly!\n") + 
            string("*\n") + 
            string("* Copyright (c) 2003, Christoph Bichlmeier\n") + 
            string("* All rights reserved.\n") + 
            string("*\n") + 
            string("* Redistribution and use in source and binary forms, with or without\n") + 
            string("* modification, are permitted provided that the following conditions\n") + 
            string("* are met:\n") + 
            string("* 1. Redistributions of source code must retain the above copyright\n") + 
            string("*    notice, this list of conditions and the following disclaimer.\n") + 
            string("* 2. Redistributions in binary form must reproduce the above copyright\n") + 
            string("*    notice, this list of conditions and the following disclaimer in the\n") + 
            string("*    documentation and/or other materials provided with the distribution.\n") + 
            string("* 3. Neither the name of the copyright holder nor the names of contributors\n") + 
            string("*    may be used to endorse or promote products derived from this software\n") + 
            string("*    without specific prior written permission.\n") + 
            string("*\n") + 
            string("* ======================================================================\n") + 
            string("*\n") + 
            string("* THIS SOFTWARE IS PROVIDED BY THE AUTHORS \'\'AS IS\'\' AND ANY EXPRESS\n") + 
            string("* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n") + 
            string("* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n") + 
            string("* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE\n") + 
            string("* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n") + 
            string("* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n") + 
            string("* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR\n") + 
            string("* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,\n") + 
            string("* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE\n") + 
            string("* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,\n") + 
            string("* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n") + 
            string("*/\n") + 
            string("\n") + 
            string("/* SHA256 logical functions */\n") + 
            string("function rotateRight(n,x) {\n") + 
            string("    return ((x >>> n) | (x << (32 - n)));\n") + 
            string("}\n") + 
            string("function choice(x,y,z) {\n") + 
            string("    return ((x & y) ^ (~x & z));\n") + 
            string("}\n") + 
            string("function majority(x,y,z) {\n") + 
            string("    return ((x & y) ^ (x & z) ^ (y & z));\n") + 
            string("}\n") + 
            string("function sha256_Sigma0(x) {\n") + 
            string("    return (rotateRight(2, x) ^ rotateRight(13, x) ^ rotateRight(22, x));\n") + 
            string("}\n") + 
            string("function sha256_Sigma1(x) {\n") + 
            string("    return (rotateRight(6, x) ^ rotateRight(11, x) ^ rotateRight(25, x));\n") + 
            string("}\n") + 
            string("function sha256_sigma0(x) {\n") + 
            string("    return (rotateRight(7, x) ^ rotateRight(18, x) ^ (x >>> 3));\n") + 
            string("}\n") + 
            string("function sha256_sigma1(x) {\n") + 
            string("    return (rotateRight(17, x) ^ rotateRight(19, x) ^ (x >>> 10));\n") + 
            string("}\n") + 
            string("function sha256_expand(W, j) {\n") + 
            string("    return (W[j&0x0f] += sha256_sigma1(W[(j+14)&0x0f]) + W[(j+9)&0x0f] +\n") + 
            string("        sha256_sigma0(W[(j+1)&0x0f]));\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Hash constant words K: */\n") + 
            string("let K256 = new Array(\n") + 
            string("    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,\n") + 
            string("    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,\n") + 
            string("    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,\n") + 
            string("    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,\n") + 
            string("    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,\n") + 
            string("    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,\n") + 
            string("    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,\n") + 
            string("    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,\n") + 
            string("    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,\n") + 
            string("    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,\n") + 
            string("    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,\n") + 
            string("    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,\n") + 
            string("    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,\n") + 
            string("    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,\n") + 
            string("    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,\n") + 
            string("    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2\n") + 
            string(");\n") + 
            string("\n") + 
            string("/* global arrays */\n") + 
            string("let ihash, count, buffer;\n") + 
            string("let sha256_hex_digits = \"0123456789abcdef\";\n") + 
            string("\n") + 
            string("/* Add 32-bit integers with 16-bit operations (bug in some JS-interpreters:\n") + 
            string("overflow) */\n") + 
            string("function safe_add(x, y)\n") + 
            string("{\n") + 
            string("    let lsw = (x & 0xffff) + (y & 0xffff);\n") + 
            string("    let msw = (x >> 16) + (y >> 16) + (lsw >> 16);\n") + 
            string("    return (msw << 16) | (lsw & 0xffff);\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Initialise the SHA256 computation */\n") + 
            string("function sha256_init() {\n") + 
            string("    ihash = new Array(8);\n") + 
            string("    count = new Array(2);\n") + 
            string("    buffer = new Array(64);\n") + 
            string("    count[0] = count[1] = 0;\n") + 
            string("    ihash[0] = 0x6a09e667;\n") + 
            string("    ihash[1] = 0xbb67ae85;\n") + 
            string("    ihash[2] = 0x3c6ef372;\n") + 
            string("    ihash[3] = 0xa54ff53a;\n") + 
            string("    ihash[4] = 0x510e527f;\n") + 
            string("    ihash[5] = 0x9b05688c;\n") + 
            string("    ihash[6] = 0x1f83d9ab;\n") + 
            string("    ihash[7] = 0x5be0cd19;\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Transform a 512-bit message block */\n") + 
            string("function sha256_transform() {\n") + 
            string("    let a, b, c, d, e, f, g, h, T1, T2;\n") + 
            string("    let W = new Array(16);\n") + 
            string("\n") + 
            string("    /* Initialize registers with the previous intermediate value */\n") + 
            string("    a = ihash[0];\n") + 
            string("    b = ihash[1];\n") + 
            string("    c = ihash[2];\n") + 
            string("    d = ihash[3];\n") + 
            string("    e = ihash[4];\n") + 
            string("    f = ihash[5];\n") + 
            string("    g = ihash[6];\n") + 
            string("    h = ihash[7];\n") + 
            string("\n") + 
            string("    /* make 32-bit words */\n") + 
            string("    for(let i=0; i<16; i++)\n") + 
            string("        W[i] = ((buffer[(i<<2)+3]) | (buffer[(i<<2)+2] << 8) | (buffer[(i<<2)+1]\n") + 
            string("            << 16) | (buffer[i<<2] << 24));\n") + 
            string("\n") + 
            string("    for(let j=0; j<64; j++) {\n") + 
            string("        T1 = h + sha256_Sigma1(e) + choice(e, f, g) + K256[j];\n") + 
            string("        if(j < 16) T1 += W[j];\n") + 
            string("        else T1 += sha256_expand(W, j);\n") + 
            string("        T2 = sha256_Sigma0(a) + majority(a, b, c);\n") + 
            string("        h = g;\n") + 
            string("        g = f;\n") + 
            string("        f = e;\n") + 
            string("        e = safe_add(d, T1);\n") + 
            string("        d = c;\n") + 
            string("        c = b;\n") + 
            string("        b = a;\n") + 
            string("        a = safe_add(T1, T2);\n") + 
            string("    }\n") + 
            string("\n") + 
            string("    /* Compute the current intermediate hash value */\n") + 
            string("    ihash[0] += a;\n") + 
            string("    ihash[1] += b;\n") + 
            string("    ihash[2] += c;\n") + 
            string("    ihash[3] += d;\n") + 
            string("    ihash[4] += e;\n") + 
            string("    ihash[5] += f;\n") + 
            string("    ihash[6] += g;\n") + 
            string("    ihash[7] += h;\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Read the next chunk of data and update the SHA256 computation */\n") + 
            string("function sha256_update(data, inputLen) {\n") + 
            string("    let i, index, curpos = 0;\n") + 
            string("    /* Compute number of bytes mod 64 */\n") + 
            string("    index = ((count[0] >> 3) & 0x3f);\n") + 
            string("    let remainder = (inputLen & 0x3f);\n") + 
            string("\n") + 
            string("    /* Update number of bits */\n") + 
            string("    if ((count[0] += (inputLen << 3)) < (inputLen << 3)) count[1]++;\n") + 
            string("    count[1] += (inputLen >> 29);\n") + 
            string("\n") + 
            string("    /* Transform as many times as possible */\n") + 
            string("    for(i=0; i+63<inputLen; i+=64) {\n") + 
            string("        for(let j=index; j<64; j++)\n") + 
            string("            buffer[j] = data.charCodeAt(curpos++);\n") + 
            string("        sha256_transform();\n") + 
            string("        index = 0;\n") + 
            string("    }\n") + 
            string("\n") + 
            string("    /* Buffer remaining input */\n") + 
            string("    for(let j=0; j<remainder; j++)\n") + 
            string("        buffer[j] = data.charCodeAt(curpos++);\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Finish the computation by operations such as padding */\n") + 
            string("function sha256_final() {\n") + 
            string("    let index = ((count[0] >> 3) & 0x3f);\n") + 
            string("    buffer[index++] = 0x80;\n") + 
            string("    if(index <= 56) {\n") + 
            string("        for(let i=index; i<56; i++)\n") + 
            string("            buffer[i] = 0;\n") + 
            string("    } else {\n") + 
            string("        for(let i=index; i<64; i++)\n") + 
            string("            buffer[i] = 0;\n") + 
            string("        sha256_transform();\n") + 
            string("        for(let i=0; i<56; i++)\n") + 
            string("            buffer[i] = 0;\n") + 
            string("    }\n") + 
            string("    buffer[56] = (count[1] >>> 24) & 0xff;\n") + 
            string("    buffer[57] = (count[1] >>> 16) & 0xff;\n") + 
            string("    buffer[58] = (count[1] >>> 8) & 0xff;\n") + 
            string("    buffer[59] = count[1] & 0xff;\n") + 
            string("    buffer[60] = (count[0] >>> 24) & 0xff;\n") + 
            string("    buffer[61] = (count[0] >>> 16) & 0xff;\n") + 
            string("    buffer[62] = (count[0] >>> 8) & 0xff;\n") + 
            string("    buffer[63] = count[0] & 0xff;\n") + 
            string("    sha256_transform();\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Split the internal hash values into an array of bytes */\n") + 
            string("function sha256_encode_bytes() {\n") + 
            string("    let j=0;\n") + 
            string("    let output = new Array(32);\n") + 
            string("    for(let i=0; i<8; i++) {\n") + 
            string("        output[j++] = ((ihash[i] >>> 24) & 0xff);\n") + 
            string("        output[j++] = ((ihash[i] >>> 16) & 0xff);\n") + 
            string("        output[j++] = ((ihash[i] >>> 8) & 0xff);\n") + 
            string("        output[j++] = (ihash[i] & 0xff);\n") + 
            string("    }\n") + 
            string("    return output;\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Get the internal hash as a hex string */\n") + 
            string("function sha256_encode_hex() {\n") + 
            string("    let output = new String();\n") + 
            string("    for(let i=0; i<8; i++) {\n") + 
            string("        for(let j=28; j>=0; j-=4)\n") + 
            string("            output += sha256_hex_digits.charAt((ihash[i] >>> j) & 0x0f);\n") + 
            string("    }\n") + 
            string("    return output;\n") + 
            string("}\n") + 
            string("\n") + 
            string("/* Main function: returns a hex string representing the SHA256 value of the\n") + 
            string("given data */\n") + 
            string("function sha256_digest(data) {\n") + 
            string("    sha256_init();\n") + 
            string("    sha256_update(data, data.length);\n") + 
            string("    sha256_final();\n") + 
            string("    return sha256_encode_hex();\n") + 
            string("}\n")
        , mimeTypeOfFile(ControllerPath));
    }

    ContentGeneratorDefineS(request.getURLPath() == ControllerPath, controllerResponse(request))

}
