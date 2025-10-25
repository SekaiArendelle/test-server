import _request

response = _request.post_http("localhost", "this/is/path", {"this-is-a": "header"}, b"{\"this is\":\"body\"}", 6699)
print(response)
