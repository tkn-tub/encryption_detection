import http.server, ssl
import sys 
server_address = ('0.0.0.0', int(sys.argv[1]))
httpd = http.server.HTTPServer(server_address, http.server.SimpleHTTPRequestHandler)
httpd.socket = ssl.wrap_socket(httpd.socket,
                               server_side=True,
                               certfile='CreatedCertificate.pem',
                               ssl_version=ssl.PROTOCOL_TLS)
httpd.serve_forever()
