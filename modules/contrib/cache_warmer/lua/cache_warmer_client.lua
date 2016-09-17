-- cache_warmer_client.lua --- Make a series of requests in parallel using the Nginx Embedded Lua
--                             module provided function ngx.location.capture_multi.
--                             (See http://wiki.nginx.org/HttpLuaModule#ngx.location.capture_multi).

-- Copyright (C) 2012 António P. P. Almeida <appa@perusio.net>

-- Author: António P. P. Almeida <appa@perusio.net>

-- Permission is hereby granted, free of charge, to any person obtaining a
-- copy of this software and associated documentation files (the "Software"),
-- to deal in the Software without restriction, including without limitation
-- the rights to use, copy, modify, merge, publish, distribute, sublicense,
-- and/or sell copies of the Software, and to permit persons to whom the
-- Software is furnished to do so, subject to the following conditions:

-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.

-- Except as contained in this notice, the name(s) of the above copyright
-- holders shall not be used in advertising or otherwise to promote the sale,
-- use or other dealings in this Software without prior written authorization.

-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
-- THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
-- FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
-- DEALINGS IN THE SOFTWARE.

-- This Lua code is part of the cache_warmer.

-- The http component of the socket library.
local http = require('socket.http')

-- Make the request to the given URIs.
function cache_warmer_make_request(uri)
   -- Make the request and get the results.
   local response, status, headers = http.request {
      url = uri,
      method = 'HEAD',
      -- Use a different User-Agent.
      headers = { ['user-agent'] = 'Nginx Cache Warmer' },
      redirect = true -- follow redirects
                                                  }
   return response, status, headers
end -- cache_warmer_make_request

-- Issue the request given the argument 'u' which is the full URI to
-- be hit.
cache_warmer_make_request(ngx.var.arg_u)
