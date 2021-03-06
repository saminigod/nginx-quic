
/*
 * Copyright (C) sunlei
 */

#ifndef QUIC_NGX_HTTP_STREAM_H_
#define QUIC_NGX_HTTP_STREAM_H_

#include "net/third_party/quiche/src/quic/tools/quic_simple_server_stream.h"

namespace quic {

#define MAX_CHUNKED_BUFFERED_SIZE 32
  
// All this does right now is aggregate data, and on fin, send an HTTP
// response.
class QuicNgxHttpStream : public QuicSimpleServerStream {
 public:
  QuicNgxHttpStream(QuicStreamId id,
                  QuicSpdySession* session,
                  StreamType type,
                  QuicSimpleServerBackend* quic_simple_server_backend);
  QuicNgxHttpStream(PendingStream* pending,
                  QuicSpdySession* session,
                  StreamType type,
                  QuicSimpleServerBackend* quic_simple_server_backend);
  QuicNgxHttpStream(const QuicNgxHttpStream&) = delete;
  QuicNgxHttpStream& operator=(const QuicNgxHttpStream&) = delete;
  ~QuicNgxHttpStream() override;

  bool SendHttpHeaders(const char*data, int len);
  bool SendHttpbody(const char*data, int len);
  
  void set_ngx_connection(void* ngx_connection) {
    ngx_connection_ = ngx_connection;
  }
  void* get_ngx_connection() { return ngx_connection_; }
  std::string get_peer_ip();
  struct sockaddr_storage get_peer_address();
    std::string get_self_ip();
  struct sockaddr_storage get_self_address();
  bool get_send_header() { return is_send_header_; }
  
  // bool OnStreamFrameAcked(QuicStreamOffset offset,
  //                         QuicByteCount data_length,
  //                         bool fin_acked,
  //                         QuicTime::Delta ack_delay_time,
  //                         QuicByteCount* newly_acked_length) override;

 private:
  void* ngx_connection_;  // Not owned.  it is ngx_connection_t
  int content_length_;
  int had_send_length_;
  bool is_http_chunked_;
  int http_chunked_step_; // 0-length, 1-data, 2-end(\r\n)
  bool fin_;
  bool is_send_header_;

  char chunked_buffered_[MAX_CHUNKED_BUFFERED_SIZE];
  int chunked_buffered_size_;
};

}  // namespace quic

#endif  // QUIC_NGX_HTTP_STREAM_H_
