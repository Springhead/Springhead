//---------------------------------------------------------------------------
/*
                 ソケットのラッパ Ver.1.3.2  by Gimite 市川
  
  TCP/IPソケットのラッパです。Windows（WinSock2）とUNIX（POSIX Socket）に対
  応しています。
  
  Gimite 市川 <gimite@mx12.freecom.ne.jp>
  
*/
//---------------------------------------------------------------------------

#ifndef GIMITE_socketH
#define GIMITE_socketH
//---------------------------------------------------------------------------
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define GIMITE_WIN32
#endif
//---------------------------------------------------------------------------
#include <iostream>
#include <ios>//streamsize
#include <streambuf>//basic_streambuf
#include <string>//char_traits, basic_string
#include <vector>//vector

struct in_addr;


/*#ifdef GIMITE_WIN32
#  //include <WinSock2.h>
#  //pragma comment(lib,"ws2_32.lib")
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <unistd.h>
#endif*/
//---------------------------------------------------------------------------
namespace gimite{
//---------------------------------------------------------------------------

#ifdef GIMITE_WIN32
	//typedef SOCKET socket_t;
	typedef unsigned __int64 socket_t;
	typedef int sock_result_t;
	typedef int sock_len_t;
	typedef unsigned long sock_uint32_t;
#else
	typedef int socket_t;
	typedef ssize_t sock_result_t;
	typedef int sock_len_t;
	typedef u_int32_t sock_uint32_t;
#endif


//---------------------------------------------------------------------------
//ソケットシステムを初期化
  
bool startup_socket();

//---------------------------------------------------------------------------
//ソケットシステムを後始末
void cleanup_socket();



//---------------------------------------------------------------------------
//IPアドレス

class ip_address
{
public:
  explicit ip_address(sock_uint32_t addr);
  explicit ip_address(const in_addr& addr);

  ip_address(const std::string& host);

  ip_address(const char* host);

  ~ip_address();
  
  ip_address& operator=(const std::string& host);
  ip_address& operator=(const char* host);
  //アドレスを表すin_addr構造体
  in_addr* addr()const;
  //アドレスを表す32ビット整数
  sock_uint32_t as_int()const;
  
private:
  in_addr* addr_;
  
  //ホスト名orIPの文字列表現からIPアドレスを得る
  void solve_address(const char* host);
  
  //比較、同値判定。
  friend bool operator<(const ip_address& lhs, const ip_address& rhs);
	friend bool operator>(const ip_address& lhs, const ip_address& rhs);
	friend bool operator<=(const ip_address& lhs, const ip_address& rhs);
	friend bool operator>=(const ip_address& lhs, const ip_address& rhs);
	friend bool operator==(const ip_address& lhs, const ip_address& rhs);
	friend bool operator!=(const ip_address& lhs, const ip_address& rhs);
  
  //stream出力。
  friend std::ostream& operator<<(std::ostream& os, const ip_address& addr);
  
};

//---------------------------------------------------------------------------
//IPアドレス+ポート番号

struct socket_address
{
	socket_address();
	explicit socket_address(ip_address& i, int p);
	~socket_address();

  //IPアドレス
  ip_address* ip;
  //ポート番号
  int port;
  
  //比較、同値判定。
  friend bool operator<(const socket_address& lhs, const socket_address& rhs);
  friend bool operator>(const socket_address& lhs, const socket_address& rhs);
  friend bool operator<=(const socket_address& lhs, const socket_address& rhs);
  friend bool operator>=(const socket_address& lhs, const socket_address& rhs);
  friend bool operator==(const socket_address& lhs, const socket_address& rhs);
  friend bool operator!=(const socket_address& lhs, const socket_address& rhs);
  //stream出力。
  friend std::ostream& operator<<(std::ostream& os, const socket_address& saddr);
	
};

//---------------------------------------------------------------------------
//ストリームソケット用streambuf

class socket_streambuf : public std::streambuf
{
  typedef std::streambuf::int_type int_type;
  typedef std::streambuf::traits_type traits_type;
  
public:
  explicit socket_streambuf(socket_t sock, std::size_t buf_size);
  socket_streambuf(const ip_address& host, int port, std::size_t buf_size);
  virtual ~socket_streambuf();
  //接続
  bool open(const ip_address& host, int port);
  //切断
  void close();
  //ソケットハンドルを閉じずに解放
  socket_t release();
  //送信
  int send(const void* buffer, int size);
  //受信
  int recv(void* buffer, int size);
  //正常に接続されてるか
  bool is_open()const;
  //ソケットハンドル
  socket_t socket()const;

protected:
  //1文字送信
  virtual int_type overflow(int_type c);
  //n文字送信
  virtual std::streamsize xsputn(const char_type* s, std::streamsize n);
  //受信バッファが空になった
  virtual int_type underflow();

private:
  socket_t sock_;
  std::vector<char> buffer_;

};

//---------------------------------------------------------------------------
//ストリームソケット用stream

class socket_stream: public std::iostream
{
public:
  //すぐには接続しないコンストラクタ
  socket_stream();
  //すぐに接続するコンストラクタ
  socket_stream(const ip_address& host, int port);
  //既存のソケットを指定するコンストラクタ
  explicit socket_stream(socket_t sock);
  //デストラクタ
  virtual ~socket_stream();
  //接続
  void open(const ip_address& host, int port);
  //切断
  void close();
  //ソケットハンドルを閉じずに解放
  socket_t release();
  //送信
  int send(const void* buffer, int size);
  //受信
  int recv(void* buffer, int size);
  //ソケットハンドル
  socket_t socket()const;
  
private:
  socket_streambuf sbuf_;

};

//---------------------------------------------------------------------------
//バインドして使うソケット

class bound_socket
{
public:
  //通常のコンストラクタ
  explicit bound_socket(int type);
  //既存のソケットハンドルを使うコンストラクタ
  bound_socket(int type, socket_t sock, bool is_bound);
  //デストラクタ
  virtual ~bound_socket();
  //ソケットをクローズ
  void close();
  //ソケットハンドルを解放
  socket_t release();
  //既存のソケットを指定
  void socket(socket_t sock, bool is_bound);
  //ソケットのタイプ
  int type()const;
  //ソケットハンドル
  socket_t socket()const;
  //正しくバインドされているか
  operator const void*()const;
  bool operator!()const;

protected:
  //バインド
  bool pbind(int port);
  
private:
  const int type_;
  socket_t sock_;
  bool is_bound_;
  
};

//---------------------------------------------------------------------------
//サーバ用ストリームソケット

class server_stream_socket: public bound_socket
{
public:
  //すぐにはバインドしないコンストラクタ
  server_stream_socket();
  //すぐにバインドするコンストラクタ
  explicit server_stream_socket(int port, int backlog);
  //既存のソケットハンドルを使うコンストラクタ
  server_stream_socket(socket_t sock, bool is_bound);
  //バインド
  bool bind(int port, int backlog);
  //接続要求を受け入れる
  socket_t accept();
  
};

//---------------------------------------------------------------------------
//ダイアグラムソケット

class diagram_socket: public bound_socket
{
public:
  //すぐにはバインドしないコンストラクタ
  diagram_socket();
  //すぐにバインドするコンストラクタ
  explicit diagram_socket(int port);
  //既存のソケットハンドルを使うコンストラクタ
  diagram_socket(socket_t sock, bool is_bound);
  //バインド
  bool bind(int port);
  //データを受信
  int recvfrom(void* buffer, int size, socket_address* addr = 0, int flags = 0);
  //データを送信
  int sendto(const void* buffer, int size, const socket_address* addr, int flags);
  
};

//---------------------------------------------------------------------------
}//namespace gimite
//---------------------------------------------------------------------------

#endif
