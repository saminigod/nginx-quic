
/*
 * Copyright (C) sunlei
 */

#ifndef QUIC_NGX_HTTP_BACKEND_H_
#define QUIC_NGX_HTTP_BACKEND_H_

#include <list>
#include <map>
#include <memory>
#include <vector>

#include "net/third_party/quiche/src/quic/core/http/spdy_utils.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_containers.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_mutex.h"
#include "net/third_party/quiche/src/common/platform/api/quiche_string_piece.h"
#include "net/third_party/quiche/src/quic/tools/quic_backend_response.h"
#include "net/third_party/quiche/src/quic/tools/quic_simple_server_backend.h"
#include "net/third_party/quiche/src/quic/tools/quic_url.h"
#include "net/third_party/quiche/src/spdy/core/spdy_framer.h"

#include "quic_ngx_http_interface.h"

namespace quic {

class QuicNgxHttpServer;
  
class QuicNgxHttpBackend : public QuicSimpleServerBackend {
 public:
  QuicNgxHttpBackend();
  QuicNgxHttpBackend(const QuicNgxHttpBackend&) = delete;
  QuicNgxHttpBackend& operator=(const QuicNgxHttpBackend&) = delete;
  ~QuicNgxHttpBackend() override;




  // Implements the functions for interface QuicSimpleServerBackend
  bool InitializeBackend(const std::string& cache_directory) override;
  bool IsBackendInitialized() const override;
  void FetchResponseFromBackend(
      const spdy::SpdyHeaderBlock& request_headers,
      const std::string& request_body,
      QuicSimpleServerBackend::RequestHandler* quic_server_stream) override;
  void CloseBackendResponseStream(
      QuicSimpleServerBackend::RequestHandler* quic_server_stream) override;

  void set_ngx_connection(void *ngx_connection) {
    cur_ngx_connection_ = ngx_connection;
  }
  void set_ngx_args(RequestHttpQuic2Ngx request_quic_2_ngx,
                    SetStreamForNgx set_stream_for_ngx) {
    request_quic_2_ngx_ = request_quic_2_ngx;
    set_stream_for_ngx_ = set_stream_for_ngx;
  }
  QuicNgxHttpServer *get_server() {
    return quic_ngx_server_;
  }
  void set_server(QuicNgxHttpServer *server) {
    quic_ngx_server_ = server;
  }
  
 private:

  bool cache_initialized_;
  void *cur_ngx_connection_; // unowned;
  RequestHttpQuic2Ngx request_quic_2_ngx_;
  SetStreamForNgx set_stream_for_ngx_;

  QuicNgxHttpServer *quic_ngx_server_; // unowned;
};

}  // namespace quic

#endif  // QUIC_NGX_HTTP_BACKEND_H_
