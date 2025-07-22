#include "webserver.h"
#include <string.h>
#include <stdlib.h>
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

typedef struct {
    char response[4096];
    size_t len;
    size_t sent;
} http_state;

static struct tcp_pcb *_server;
static request_handler _handler;
static http_state _state;

static err_t on_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    _state.sent += len;
    if (_state.sent >= _state.len) tcp_close(tpcb);
    return ERR_OK;
}

static err_t on_receive(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }
    char *route = p->payload;
    char *response = _handler(route);
    _state.sent = 0;
    if (strstr(response, "<html") != NULL) {
       _state.len = snprintf(
             _state.response, sizeof(_state.response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            (int)strlen(response), response
        );
    } else if (strlen(response) == 0) {
         _state.len = snprintf(
            _state.response, sizeof(_state.response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n"
        );
    } else {
        _state.len = snprintf(
            _state.response, sizeof(_state.response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            (int)strlen(response), response
        );
    }
    tcp_write(tpcb, _state.response, _state.len, TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    tcp_sent(tpcb, on_sent);
    pbuf_free(p);
    return ERR_OK;
}

static err_t on_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, on_receive);
    return ERR_OK;
}

int webserver_create(int port, request_handler handler) {
    _server = tcp_new();
    if (!_server) return -1;
    if (tcp_bind(_server, IP_ADDR_ANY, port) != ERR_OK) return -1;
    _server = tcp_listen(_server);
    tcp_accept(_server, on_accept);
    _handler = handler;
}