#pragma once

typedef char* (*request_handler)(char* route);

int webserver_create(int port, request_handler handler);