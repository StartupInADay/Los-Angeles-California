-- cache_warmer_requests.lua --- Performs the location multi capture
--                               to issue the requests in parallel.

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

-- First we grab the POST data.
ngx.req.read_body() -- read the request body
local post_data = ngx.req.get_post_args() -- capture the arguments.
local base_uri = post_data['base_uri'] -- grab the base URI

-- Release the base URI entry from the POST data table.
post_data['base_uri'] = nil

local requests = {} -- requests table

-- -- Building the location for making the parallel requests.
function build_req_uri(base_uri, uri)
   return string.format('/parallel-reqs?u=%s/%s', base_uri, uri)
end -- build_req_uri

-- Loop over the post_data table (contains the URIs to be hit).
for _, u in pairs(post_data) do
   -- All requests are HEAD requests.
   table.insert(requests, { build_req_uri(base_uri, u), { method = ngx.HTTP_HEAD }})
end

-- Issue the requests and store the responses in a table.
local responses = { ngx.location.capture_multi(requests) }

-- Process the responses.
for _, r in pairs(responses) do
   ngx.say(r.status) -- get the status only (HEAD)
end
