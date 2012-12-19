////////////////////////////////////////////////////////////////////
//
// Lacewing networking platform
// http://lacewing.aquadasoft.com
//
// Copyright © Jamie McLaughlin, 2007 - 2010
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL JAMIE MCLAUGHLIN
// OR AQUADASOFT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Contact: jamie@aquadasoft.com
// Website: http://www.aquadasoft.com
//
// Special thanks to Mathias Kærlev, Jeff Vance, Chris Branch, Lukas Meller, Mark Hendriks, Jean Villy Edberg and Matt Luard
//
// To connect from ActionScript Lacewing, a Flash Player policy server must be running, hosting an XML file that allows access
// More information on Flash Player policies - http://www.adobe.com/devnet/flashplayer/articles/socket_policy_files.html
//
// Utilities:
//
// Lacewing::Sync - synchronise access to data between different threads
// Lacewing::Event - trigger events in one thread from another
// Lacewing::Ping - send ICMP ping requests to a hostname or IP address
// Lacewing::LAN - enumerate devices on a local area network
// Lacewing::FlashPlayerPolicy - host a flash player policy file to allow cross server connections from ActionScript Lacewing
//
// Raw sockets:
//
// Lacewing::SocketClient - lightweight multithreaded socket client
// Lacewing::SocketServer - lightweight multithreaded socket server with secure socket support
// Lacewing::UDP - fast, connectionless communication using UDP rather than TCP
//
// Protocols:
//
// Lacewing::Client - complete implementation of the Lacewing protocol (client-side)
// Lacewing::Server - complete implementation of the Lacewing protocol (server-side)
// Lacewing::Webserver - host an embedded webserver with HTTP/HTTPS support, controlled by your C++ code
// Lacewing::IRC - an IRC client in the spirit of Lacewing::Client - unfinished
//
////////////////////////////////////////////////////////////////////

//
// Include guard
//

#ifndef LacewingIncluded
#define LacewingIncluded

//
// Check for a C++ compiler
//

#ifndef __cplusplus
#pragma error("Lacewing is not compatible with C - please use a C++ compiler")
#else

//
// Define LacewingFunction to import functions from the DLL
//

#ifndef LacewingFunction
#define LacewingFunction __declspec(dllimport)

//
// Automatically link with Lacewing.lib if Lacewing_NoAutoLink isn't defined
//

#ifndef Lacewing_NoAutoLink
#pragma comment(lib, "Lacewing.lib")
#endif
#endif

//
// VC6 warning level 4 throws "warning C4706: assignment within conditional expression" for Lacewing loops
//

#ifdef Lacewing_FixL4
#pragma warning(disable:4706)
#endif

//
// Lacewing namespace
//

namespace Lacewing
{

//
// The Event class for the event functions
//

class Event;

///////////////////////////////
///
/// Global functions
///
//////////////////////////////

//
// Retrieve the version of the Lacewing.dll that has been loaded
//

LacewingFunction const char * Version();

//
// Initialise Lacewing. Set MFC to true to use AfxBeginThread for multithreading.
// All Lacewing classes will call this upon construction. Has no effect if it has already been called
//

LacewingFunction bool Initialise(bool MFC = false);

//
// Free Lacewing
//

LacewingFunction void Free();

///////////////////////////////
///
/// Utility functions
///
//////////////////////////////

namespace Utility
{
    //
    // Start a thread. Uses _beginthreadex if Lacewing was initialised with MFC = false, or AfxBeginThread
    // if Lacewing was initialised with MFC = true
    //

    LacewingFunction bool StartThread(void * Function, void * Parameter = 0);

    //
    // Pause the current thread for a number of milliseconds
    // Not useful for very much (except spinning) but provided for compatibility
    //

    LacewingFunction void Pause(int Milliseconds);

    //
    // Wait for a single event to trigger. Returns true if the event signaled or false if the timeout elapsed
    //

    LacewingFunction bool WaitEvent(Event &Event, int Timeout = -1);

    //
    // Wait for multiple events to trigger. Returns true if the event signaled or false if the timeout elapsed
    //

    LacewingFunction bool WaitEvents(bool All, int Timeout, ...);

    //
    // Resolve an string IP address or hostname into a 4 byte address (long integer)
    // This is blocking and may well be slow; use with caution from your main thread
    //

    LacewingFunction int Resolve(const char * Address);

    //
    // Pause the current thread until the console is closed (for console applications)
    // Useful for quick tests, but it's generally not a good idea to use a console under Windows
    //

    LacewingFunction void SuspendConsole();

    //
    // Get the last modified time for a file as a UNIX time (seconds since 1970)
    // Useful for handling file modified times with Lacewing::Webserver
    //

    LacewingFunction __int64 LastModified(const char * Filename);
}

///////////////////////////////
///
///  Enumerations
///
//////////////////////////////

// Transmission modes

// These specify how data is to be (or was) transmitted. Send uses TCP (guaranteed but slow) and
// blast uses UDP (non-guaranteed but fast)

namespace TransmissionMode
{
    enum Type
    {
        Send                         = 0,
        Blast                        = 1,
    };

    LacewingFunction const char * ToString(Type);
};

//
// Control types
//

// These are returned from the handlers called by Lacewing to specify how behaviour
// should be controlled. Lacewing::Control::Cancel will cancel the default behaviour,
// and Lacewing::Control::Passive will have no effect on the default behaviour.

namespace Control
{
    enum Type
    {
        Cancel                         = 0,
        Passive                        = 1,
    };

    LacewingFunction const char * ToString(Type);
};

//
// Stream controllers
//

// These can change the behaviour of stream operators in Lacewing classes that support them

namespace Stream
{
    enum Type
    {
        // Lock or unlock the stream to avoid multiple threads accessing it the same time
        // (ex: Socket << Lock << "Hello" << Unlock)

        Lock                          = 0,
        Unlock                        = 1,

        // Set 0 padding for any numbers to follow - has no effect in binary mode
        // (ex: Socket << Padding << 2 << 1; would send "02")

        Padding                       = 2,
    };

    LacewingFunction const char * ToString(Type);
}

//
// Protocol implementations
//

// The different implementations of the Lacewing protocol that can interact with the
// Client and Server. If you do your own implementation, please let me know and I'll
// add it to the next protocol revision - you can use "Unspecified" for now.

namespace ProtocolImplementation
{
    enum Type
    {
        DLL,
        Python,
        Flash,
        BlitzMax,
        Java,
        Utility,
        WebSocket,
        Unspecified,
    };

    LacewingFunction const char * ToString(Type);
}


//
// Handler modes
//

namespace HandlerMode
{
    enum Type
    {
        // A handler registered with Lacewing::HandlerMode::Trigger will be called immediately
        // by the thread in which the event occurs. This has the least impact on performance
        // when allowing handlers to control behaviour, but the handler could be operating in any
        // thread, so synchronisation objects may have to be used (see Lacewing::Sync)

        Trigger                        = 0,

        // A handler registered with Lacewing::HandlerMode::Queue will be added to a queue of
        // handlers when it occurs, and then the handler will be called when ProcessHandlers()
        // is called. This has the least impact on performance while remaining thread-safe,
        // but handlers have no way of controlling behaviour (as they will be called too late)

        Queue                          = 1,

        // A handler registered with Lacewing::HandlerMode::Suspend will suspend the thread until
        // the handler is called by ProcessHandlers(). This has a large impact on performance,
        // but is thread-safe and allows handlers to control behaviour. If a handler is ready
        // to be called in the same thread it was registered in, it will trigger immediately
        // rather than wait for ProcessHandlers()

        Suspend                        = 2,
    };

    LacewingFunction const char * ToString(Type);
}

//
// Reasons a client may be denied from a Lacewing server
//

namespace DenyStatus
{
    enum Type
    {
        NoReasonGiven                 = 0,
        ServerFull                    = 1,
        IPBanned                      = 2,
        InvalidName                   = 3,
        InvalidChannel                = 4,
        UnsupportedVersion            = 6,
        NoPingResponse                = 7,
        NameAlreadyTaken              = 8,
        ProtocolFailure               = 9,
        NotDenied                     = 10,
        UnexpectedPacket              = 11,
    };

    LacewingFunction const char * ToString(Type);
}

//
// Error codes
//

namespace ErrorCode
{

    enum Type
    {
        ErrorBindingPort                    = 20001,
        ErrorListening                      = 20002,
        ErrorCreatingSocket                 = 20003,
        ErrorCreatingEvent                  = 20004,
        ErrorSelectingEvent                 = 20005,
        ErrorReceiving                      = 20006,
        ErrorAccepting                      = 20007,
        ErrorSending                        = 20008,
        ErrorAddingClient                   = 20009,
        ErrorResolvingHostname              = 20010,
        ErrorConnecting                     = 20011,
        NoChannel                           = 20013,
        NoName                              = 20014,
        AlreadyConnected                    = 20015,
        ErrorConnectionTime                 = 20016,
        IncorrectLoopUsage                  = 20017,
        InvalidPort                         = 20018,
        ErrorReadingAddress                 = 20019,
        NotConnected                        = 20020,
        ProxyRejectedConnection             = 20021,
        ProxyRequiresIdentd                 = 20022,
        ProxyIdentdFailed                   = 20023,
        PortAlreadyTaken                    = 20025,
        NoHostSet                           = 20026,
        UDPHostListenConflict               = 20027,
        PacketTooLarge                      = 20028,
        ErrorCreatingICMP                   = 20029,
        ErrorEnumeratingDevices             = 20030,
        ErrorReceivingFile                  = 20031,
        ErrorSendingFile                    = 20032,
        FileNotFound                        = 20037,
        AlreadyListening                    = 20038,
        ProtocolFailure                     = 20039,
        UnsupportedOperatingSystem          = 20040,
        ErrorOpeningFile                    = 20041,
        ErrorLoadingCertificate             = 20042,
        CertificateNotLoaded                = 20043,
        SecureHandshakeFailure              = 20044,
        ErrorOpeningCertificateFile         = 20045,
        ErrorFindingCertificate             = 20046,
        ErrorAcquiringCredentialsHandle     = 20047,
        UnknownCertificateLocation          = 20048,

        Interrupted                   = 10004,
        InvalidFileHandle             = 10009,
        PermissionDenied              = 10013,
        BadAddress                    = 10014,
        InvalidArgument               = 10022,
        TooManySockets                = 10024,
        WouldBlock                    = 10035,
        InProgress                    = 10036,
        OperationAlreadyInProgress    = 10037,
        NotSocket                     = 10038,
        NoDestination                 = 10039,
        TooLong                       = 10040,
        WrongProtocol                 = 10041,
        BadOption                     = 10042,
        UnsupportedProtocol           = 10043,
        UnsupportedSocket             = 10044,
        UnsupportedOperation          = 10045,
        UnsupportedProtocolFamily     = 10046,
        UnsupportedAddressFamily      = 10047,
        PortAlreadyUsed               = 10048,
        InvalidAddress                = 10049,
        NetworkDown                   = 10050,
        NetworkUnreachable            = 10051,
        NetworkReset                  = 10052,
        ConnectionAborted             = 10053,
        ConnectionResetByPeer         = 10054,
        NoBufferSpaceAvailable        = 10055,
        SocketAlreadyConnected        = 10056,
        SocketNotConnected            = 10057,
        SocketShutdown                = 10058,
        TooManyReferences             = 10059,
        ConnectionTimedOut            = 10060,
        ConnectionRefused             = 10061,
        CannotTranslateName           = 10062,
        NameTooLong                   = 10063,
        HostDown                      = 10064,
        NoRouteToHost                 = 10065,
        DirectoryNotEmpty             = 10066,
        TooManyProcesses              = 10067,
        UserQuotaExceeded             = 10068,
        DiskQuotaExceeded             = 10069,
        StaleFileHandleReference      = 10070,
        ItemRemote                    = 10071,
        NetworkSubsystemUnavailable   = 10091,
        UnsupportedWinsockVersion     = 10092,
        WinsockNotInitialised         = 10093,
        GracefulShutdownInProgress    = 10101,
        NoMoreResults                 = 10102,
        CallCanceled                  = 10103,
        InvalidProcedureCallTable     = 10104,
        InvalidServiceProvider        = 10105,
        ProviderLoadFailed            = 10106,
        SystemCallFailed              = 10107,
        ServiceNotFound               = 10108,
        ClassTypeNotFound             = 10109,
        NoMoreLookupServiceResults    = 10110,
        LookupServiceEndCalled        = 10111,
        DatabaseQueryRefused          = 10112,
        HostNotFound                  = 11001,
        TryHostLookupAgain            = 11002,
        HostLookupFailed              = 11003,
        NoDataRecord                  = 11004,
        // FileNotFound               = 21011,
        Busy                          = 21034,
        SocketError                   = 21035,
        UDPError                      = 21036,
        WebserverError                = 21037,

        DebugCode0                    = 21001,
        DebugCode1                    = 21002,
        DebugCode2                    = 21003,
        DebugCode3                    = 21004,
        DebugCode4                    = 21005,
        DebugCode5                    = 21006,
        DebugCode6                    = 21007,
        DebugCode7                    = 21008,
        DebugCode8                    = 21009,
        DebugCode9                    = 21010,
        DebugCodeA                    = 21033,
        DebugCodeB                    = 21012,
        DebugCodeC                    = 21013,
        DebugCodeD                    = 21014,
        DebugCodeE                    = 21015,
        DebugCodeF                    = 21016,

        ExtendedDebugCode0            = 21017,
        ExtendedDebugCode1            = 21018,
        ExtendedDebugCode2            = 21019,
        ExtendedDebugCode3            = 21020,
        ExtendedDebugCode4            = 21021,
        ExtendedDebugCode5            = 21022,
        ExtendedDebugCode6            = 21023,
        ExtendedDebugCode7            = 21024,
        ExtendedDebugCode8            = 21025,
        ExtendedDebugCode9            = 21026,
        ExtendedDebugCodeA            = 21027,
        ExtendedDebugCodeB            = 21028,
        ExtendedDebugCodeC            = 21029,
        ExtendedDebugCodeD            = 21030,
        ExtendedDebugCodeE            = 21031,
        ExtendedDebugCodeF            = 21032,
        
        NoError                       = 0xFFFFFFFF,

    };

    LacewingFunction const char * ToString(Type);
}

//
// Proxy types
//

namespace Proxy
{
    enum Type
    {
        Socks4,

        // Currently not implemented
        Socks4A,
        Socks5,
    };

    LacewingFunction const char * ToString(Type);
};

//
// FTP transfer types
//

namespace FTPTransferType
{
    enum Type
    {
        ASCII,
        EBCDIC,
        Binary
    };

    LacewingFunction const char * ToString(Type);
}

///////////////////////////////
///
///  Classes
///
//////////////////////////////

class Error
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction Error();
    LacewingFunction ~Error();

    //
    // Loop through the error codes contained in this error (highest level first)
    //
    
    LacewingFunction int ErrorCodeLoop(int ID = 0);
    LacewingFunction Lacewing::ErrorCode::Type ErrorCodeLoopIndex(int ID);

    /*
        for(int ID = Error.ErrorCodeLoop(); ID; ID = Error.ErrorCodeLoop(ID))
        {
            Lacewing::ErrorCode::Type ErrorCode = Error.ErrorCodeLoopIndex(ID);
            
            // cout << "Error: " << Lacewing::ErrorCode::ToString(ErrorCode) << endl;
        }

    */

    //
    // Add a new error code to this error (as the highest)
    //

    LacewingFunction void Add(Lacewing::ErrorCode::Type Error);

    //
    // Clone the error - returns a pointer that must be freed
    //

    LacewingFunction Lacewing::Error * Clone();
 
    //
    // Convert this error (all error codes) to a single, human readable string
    //
    
    LacewingFunction const char * ToString();

    //
    // ToString as an operator
    //

    LacewingFunction operator const char * ();

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class Sync
{

public:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

    //
    // Constructor/destructor
    //

    LacewingFunction Sync();
    LacewingFunction ~Sync();

    //
    // Class to safely lock/unlock the synchronisation object, for read access ONLY.
    // Do not write to an resource you only own a ReadLock for. Do not create a Lock
    // when you already have a ReadLock. You may create a ReadLock when you already
    // have a Lock.
    //

    struct ReadLock
    {
        void * InternalTag;

        LacewingFunction ReadLock(Lacewing::Sync &Object);
        LacewingFunction ~ReadLock();
    };
    
    //
    // Class to safely lock/unlock the synchronisation object, for read and write access.
    // Do not create a Lock when you already have a ReadLock, unless you pass it as the
    // second parameter to the constructor of Lock. You may create a ReadLock when you
    // already have a Lock.
    //

    struct Lock
    {
        void * InternalTag;

        LacewingFunction Lock(Lacewing::Sync &Object);
        LacewingFunction Lock(Lacewing::Sync &Object, ReadLock &Upgrade);

        LacewingFunction ~Lock();
    };

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};


class Event
{
public:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

    //
    // Constructor/destructor
    //

    LacewingFunction Event();
    LacewingFunction ~Event();

    //
    // Check if the event is signalled or not
    //

    LacewingFunction bool Check();

    //
    // Signal the event. Any thread currently using the Wait() method of this class, Lacewing::Utility::WaitEvent()
    // or Lacewing::Utility::WaitEvents() will return true.
    //

    LacewingFunction bool Signal();

    //
    // Unsignal the event
    //

    LacewingFunction void Unsignal();

    //
    // Wait for the event to become signaled. Returns true if the event signaled by Signal() or
    // false if the timeout elapsed.
    //
    
    LacewingFunction bool Wait(int Timeout = -1);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class SocketClient
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction SocketClient();
    LacewingFunction ~SocketClient();

    //
    // Connect function
    //
    
    LacewingFunction void Connect(const char * Host, int Port);
    LacewingFunction void Connect(const char * ProxyAddress, int ProxyPort, const char * Host, int Port, Proxy::Type ProxyType, const char * ProxyUserID);

    //
    // Disconnect function. If Block is true, Disconnect() will not return until disconnection is complete.
    //

    LacewingFunction void Disconnect(bool Block = true);

    //
    // Get the IP address of the server the client is connected to as a string
    //
    
    LacewingFunction const char * ServerIP();

    //
    // Get the server port the client is connected to
    //

    LacewingFunction int ServerPort();

    struct
    {

        //
        // IP of the server the client is connected to
        //

        unsigned char IP[4];

        //
        // Port the client is connected to the server on
        //
        
        int Port;

    } Address;


    //
    // Send a memory buffer to the server. If Size is not specified (or is specified as -1), the
    // buffer is assumed to be a null terminated string. It will be sent without the null terminator.
    //

    LacewingFunction bool Send(const char * Buffer, int Size = -1, bool Block = false);


    //
    // File send function
    //

    LacewingFunction bool SendFile(const char * Filename);

    //
    // Stream operators for sending various types
    //
    
    LacewingFunction SocketClient & operator << (char);
    LacewingFunction SocketClient & operator << (unsigned char);
    LacewingFunction SocketClient & operator << (short);
    LacewingFunction SocketClient & operator << (unsigned short);
    LacewingFunction SocketClient & operator << (int);
    LacewingFunction SocketClient & operator << (unsigned int);
    LacewingFunction SocketClient & operator << (const char *);
    LacewingFunction SocketClient & operator << (__int64);
    LacewingFunction SocketClient & operator << (float);
    LacewingFunction SocketClient & operator << (double);
    
    //
    // Changing stream behaviour
    //

    LacewingFunction SocketClient & operator << (Lacewing::Stream::Type);

    //
    // Enable or disable binary mode for the stream operators (default: false)
    //

    LacewingFunction void SetBinaryMode(bool Enabled = false);

    //
    // Retrieve how long this client has been connected in seconds
    //

    LacewingFunction int ConnectionTime();

    //
    // Check if the client is currently connected to a server
    //

    LacewingFunction bool Connected();

    //
    // Check if the client is in the process of trying to connect to a server
    //

    LacewingFunction bool Connecting();

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //
    
    typedef void (__cdecl * HandlerConnect)      (Lacewing::SocketClient &Client);
    typedef void (__cdecl * HandlerDisconnect)   (Lacewing::SocketClient &Client);
    typedef void (__cdecl * HandlerReceive)      (Lacewing::SocketClient &Client, char * Buffer, int Size);
    typedef void (__cdecl * HandlerError)        (Lacewing::SocketClient &Server, Lacewing::Error &);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerReceive(HandlerReceive, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};


class SocketServer
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction SocketServer();
    LacewingFunction ~SocketServer();

    //
    // Host function
    //

    LacewingFunction void Host(int Port);

    //
    // Stop hosting
    //

    LacewingFunction void Unhost();

    //
    // Check if the server is hosting
    //

    LacewingFunction bool Hosting();

    //
    // Get the port which the server is hosting on (0 if not hosting)
    //

    LacewingFunction int Port();

    //
    // Retrieve the total number of bytes the server has sent and received
    //

    LacewingFunction __int64 BytesSent();
    LacewingFunction __int64 BytesReceived();

    //
    // Load a certificate file (CER or PFX) and switch to secure mode (before hosting)
    // If you are already hosting, the server will be stopped first.
	//

    LacewingFunction bool LoadCertificateFile(const char * Filename, const char * CommonName);

    //
    // Load a certificate from the system and switch to secure mode (before hosting)
    // If you are already hosting, the server will be stopped first.
	//
    // More information on the StoreName: http://msdn.microsoft.com/en-us/library/aa388136%28v=VS.85%29.aspx
    // The predefined system stores are "MY", "Root", "Trust" and "CA"
    //
    // Location may be "CurrentService", "CurrentUser", "CurrentUserGroupPolicy", "LocalMachine", "LocalMachineEnterprise",
    // "LocalMachineGroupPolicy", "Services" or "Users"
    //

    LacewingFunction bool LoadSystemCertificate(const char * StoreName, const char * CommonName, const char * Location = "CurrentUser");


    //
    // Check if a certificate has been loaded using LoadCertificateFile or LoadSystemCertificate
    //

    LacewingFunction bool CertificateLoaded();

    //
    // Structure to represent a client on the server
    //

    struct Client
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // The address of the client as a 4 byte IP address
        //

        unsigned char IP[4];

        //
        // The port the server is connected to the client on
        //

        int Port;

        //
        // Returns the IP address of the client as a string
        //

        LacewingFunction const char * StringIP();

        //
        // Send a memory buffer to the client. If Size is not specified (or is specified as -1), the
        // buffer is assumed to be a null terminated string. It will be sent without the null terminator.
        //
        // If Block is true, Send() will not return until the buffer has been fully sent
        //

        LacewingFunction bool Send(const char * Buffer, int Size = -1, bool Block = false);

        //
        // File send function
        //

        LacewingFunction bool SendFile(const char * Filename, __int64 Offset= 0, __int64 Size = 0);

        //
        // This is only useful in secure mode (when a certificate has been loaded). It should be used when
        // sending a buffer you don't care about the contents of after sending it, as the buffer will be
        // encrypted in place. The standard Send() function, in secure mode, will copy the buffer before
        // encrypting it, so SendWritable() is much faster. When not in secure mode, SendWritable()
        // will behave the same as Send().
        //

        LacewingFunction bool SendWritable(char * Buffer, int Size = -1, bool Block = false);

        //
        // Stream operators for sending various types
        //

        LacewingFunction Client & operator << (char);
        LacewingFunction Client & operator << (unsigned char);
        LacewingFunction Client & operator << (short);
        LacewingFunction Client & operator << (unsigned short);
        LacewingFunction Client & operator << (int);
        LacewingFunction Client & operator << (unsigned int);
        LacewingFunction Client & operator << (const char *);
        LacewingFunction Client & operator << (__int64);
        LacewingFunction Client & operator << (float);
        LacewingFunction Client & operator << (double);

        //
        // Operator for changing stream behaviour
        //

        LacewingFunction Client & operator << (Lacewing::Stream::Type);

        //
        // Enable or disable binary mode for the stream operators (default: false)
        //

        LacewingFunction void SetBinaryMode(bool Enabled = false);

        //
        // Disconnect this client
        //

        LacewingFunction void Disconnect();

        //
        // Disconnect this client from within a client loop. The return value is a new loop ID - Index = Client->Disconnect(Index)
        //

        LacewingFunction int Disconnect(int LoopID);

        //
        // Retrieve how long this client has been connected in seconds
        //

        LacewingFunction int ConnectionTime();

        //
        // Retrieve the total number of bytes this client has been sent and the server has received from it
        //

        LacewingFunction __int64 BytesSent();
        LacewingFunction __int64 BytesReceived();

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // The number of clients connected to the server
    //

    LacewingFunction int ClientCount();

    //
    // Loop through the connected clients
    //

    LacewingFunction int ClientLoop(int ID = 0);
    LacewingFunction Client &ClientLoopIndex(int ID);
    
    //
    // Prematurely end the client loop
    //

    LacewingFunction void EndClientLoop(int ID);

    /*

        for(int ID = Server.ClientLoop(); ID; ID = Server.ClientLoop(ID))
        {
            Lacewing::SocketServer::Client &Client = Server.ClientLoopIndex(ID);
            
            // If leaving the client loop before the end
            Server.EndClientLoop(ID);
        }

    */

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef Lacewing::Control::Type    (__cdecl * HandlerConnect)        (SocketServer &Server, SocketServer::Client &Client);
    typedef void                       (__cdecl * HandlerDisconnect)    (SocketServer &Server, SocketServer::Client &Client);
    typedef void                       (__cdecl * HandlerReceive)        (SocketServer &Server, SocketServer::Client &Client, char * Buffer, int Size);
    typedef void                       (__cdecl * HandlerError)        (SocketServer &Server, Lacewing::Error &);
    
    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerReceive(HandlerReceive, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class UDP
{

public:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

    //
    // Constructor/destructor
    //

    LacewingFunction UDP();
    LacewingFunction ~UDP();

    //
    // Listen on a port for incoming data
    //

    LacewingFunction void Host(int Port);

    //
    // Stop listening for incoming data
    //

    LacewingFunction void Unhost();

    //
    // Get the port which is being hosted on (0 if not hosting)
    //

    LacewingFunction int Port();

    //
    // Address structure to identify the source of received data
    //

    struct Address
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // Empty constructor (if this is used, Initialise must be called before use or deletion)
        //

        LacewingFunction Address();

        //
        // Constructor/destructor
        //

        LacewingFunction Address(UDP &UDP);
        LacewingFunction ~Address();

        //
        // Set the address from a hostname
        //

        LacewingFunction void Set(const char * Hostname, int Port);

        //
        // Set the address from a SocketServer::Client
        //

        LacewingFunction void Set(SocketServer::Client &Host, int Port);

        //
        // Set the address and port from a SocketClient
        //

        LacewingFunction void Set(SocketClient &Host);
        
        //
        // Retrieve the IP address as a string
        //

        LacewingFunction const char * IP_AsString();

        //
        // Retrieve the IP address as a 4 byte integer
        //

        LacewingFunction int IP_AsInteger();

        //
        // Retrieve an individual byte of the IP address
        //

        LacewingFunction unsigned char IP_AsByte(int Index);

        //
        // Retrieve the port
        //

        LacewingFunction int Port();

        //
        // Send a UDP packet to this address
        //

        LacewingFunction void Send(const char * Buffer, int Size = -1);

        //
        // Listen for incoming UDP packets from this address. This cannot be used at the same time
        // as Host() in the same UDP object. It returns the port which is being used locally by
        // the operating system or 0 on failure.
        //

        LacewingFunction int Listen(int Port);

        //
        // Stop listening for incoming UDP packets from this address
        //

        LacewingFunction void StopListening();

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef void (__cdecl * HandlerReceive)          (UDP &UDP, Lacewing::UDP::Address &Source, char * Buffer, int Size);
    typedef void (__cdecl * HandlerError)            (UDP &UDP, Lacewing::Error &);
    
    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerReceive(HandlerReceive, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class Webserver
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

public:

    //
    // Constructor and destructor
    //
    
    LacewingFunction Webserver();
    LacewingFunction ~Webserver();

    //
    // Start the HTTP or HTTPS server on the specified port
    //
    
    LacewingFunction void Host(int Port = 80);
    LacewingFunction void HostSecure(int Port = 443);

    //
    // Stop the HTTP or HTTPS server
    //
    
    LacewingFunction void Unhost();
    LacewingFunction void UnhostSecure();

    //
    // Check if the HTTP or HTTPS server is currently running
    //

    LacewingFunction bool Hosting();
    LacewingFunction bool HostingSecure();

    //
    // Retrieve the port the HTTP or HTTPS server is currently running on
    //

    LacewingFunction int Port();
    LacewingFunction int SecurePort();

    //
    // Load a certificate file (CER or PFX) before calling HostSecure()
    // If you are already hosting, the server will be stopped first.
	//

    LacewingFunction bool LoadCertificateFile(const char * Filename, const char * CommonName);

    //
    // Load a certificate from the system and switch to secure mode before calling HostSecure()
    // If you are already hosting, the server will be stopped first.
	//
    // More information on the StoreName: http://msdn.microsoft.com/en-us/library/aa388136%28v=VS.85%29.aspx
    // The predefined system stores are "MY", "Root", "Trust" and "CA"
    //
    // Location may be "CurrentService", "CurrentUser", "CurrentUserGroupPolicy", "LocalMachine", "LocalMachineEnterprise",
    // "LocalMachineGroupPolicy", "Services" or "Users"
    //

    LacewingFunction bool LoadSystemCertificate(const char * StoreName, const char * CommonName, const char * Location = "CurrentUser");

    //
    // Check if a certificate has been loaded using LoadCertificateFile or LoadSystemCertificate
    //
    
    LacewingFunction bool CertificateLoaded();


    //
    // Retrieve the total number of bytes the HTTP or HTTPS server has sent and received
    //

    LacewingFunction __int64 BytesSent();
    LacewingFunction __int64 BytesReceived();

    LacewingFunction __int64 SecureBytesSent();
    LacewingFunction __int64 SecureBytesReceived();

    //
    // Structure to represent a request to the server
    //

    struct Request
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // Immediately close the socket used to receive the request
        //

        LacewingFunction void Disconnect();

        //
        // Redirect the request to another URL
        //

        LacewingFunction void SetRedirect(const char * URL);

		//
		// Set the status code and message for the response (eg 404, "Not Found")
		// Default is 200, "OK" unless one of the other functions changes it.
		//

		LacewingFunction void SetResponseType(int StatusCode, const char * Message);

        //
        // Set the mime type for the request
        //

        LacewingFunction void SetMimeType(const char * MimeType);

        //
        // Suggests a mime type for a file or file extension
        //

        LacewingFunction const char * SuggestMimeType(const char * Filename);

        //
        // Equivalent to SetMimeType(SuggestMimeType(Filename)). Returns the mime type used.
        //

        LacewingFunction const char * SetSuggestedMimeType(const char * Filename);

        //
        // Send data in response to the request (after calling BeginResponse)
        //

        LacewingFunction void Send(const char * Buffer, int Size = -1);

        //
        // The same as Send, but sends a file instead
        //

        LacewingFunction bool SendFile(const char * Filename);

        //
        // The same as Send, but sends a memory buffer that must remain valid until the
        // response is sent (at an unspecified time). Useful for sending large, constant memory buffers
        // as they will not be copied

        LacewingFunction void SendConstant(const char * Buffer, int Size = -1);

        //
        // Get/set the Last-Modified header for the request/response
        // Time is in UNIX format (the number of seconds since 1970)
        //

        LacewingFunction __int64 GetLastModified();
        LacewingFunction void SetLastModified(__int64 Time);

        //
        // set the response to "304 Not Modified" (do this if GetLastModified() matches the local file)
        //
        
        LacewingFunction void SetUnmodified();

        //
        // Read a header from the request
        //

        LacewingFunction const char * GetHeader(const char * Name);

        //
        // Set a cookie to be stored by the browser
        //

        LacewingFunction void SetCookie(const char * Name, const char * Value);

        //
        // Get a cookie sent by the browser
        //

        LacewingFunction const char * GetCookie(const char * Name);

        //
        // Ask the browser not to cache this response (default: cache enabled)
        //

        LacewingFunction void DisableCache();

        //
        // Set the charset for this response. The default is "UTF-8".
        //

        LacewingFunction void SetCharset(const char * Charset);

        //
        // Get an item from the POST data by name
        //

        LacewingFunction const char * GetPostItem(const char * Name);

        //
        // Get an item from the GET data by name. This will call StripGet()
        // if it has not already been called.
        //

        LacewingFunction const char * GetGetItem(const char * Name);

        //
        // Strip the GET data from the URL
		//
        // If Backup is true, a copy of the full URL (valid as long as the request object)
		// will be returned.
		//

        LacewingFunction const char * StripGet(bool Backup = false);

		//
		// Export a complete set of CGI environment variables for this request
		//

		LacewingFunction void ExportCGIEnvironment(const char * ScriptFilename);

        //
        // Enable download resuming for this request (default: disabled)
        //

        LacewingFunction void EnableDownloadResuming();

        //
        // Get the requested range beginning and end (for download resuming). If RangeBeginning() is
        // negative, RangeEnd() will be 0 and RangeBeginning() will be an offset from the end of the file.
        //

        LacewingFunction __int64 RequestedRangeBeginning();
        LacewingFunction __int64 RequestedRangeEnd();

        //
        // If responding to a range request, use this to tell the browser which range you're sending
        //

        LacewingFunction void SetOutgoingRange(__int64 Begin, __int64 End);

        //
        // Stream operators for sending various types
        //

        LacewingFunction Request & operator << (char);
        LacewingFunction Request & operator << (unsigned char);
        LacewingFunction Request & operator << (short);
        LacewingFunction Request & operator << (unsigned short);
        LacewingFunction Request & operator << (int);
        LacewingFunction Request & operator << (unsigned int);
        LacewingFunction Request & operator << (const char *);
        LacewingFunction Request & operator << (__int64);
        LacewingFunction Request & operator << (float);
        LacewingFunction Request & operator << (double);

        //
        // Changing stream behaviour
        //

        LacewingFunction SocketClient & operator << (Lacewing::Stream::Type);

        //
        // Stream operator for sending an STL string (wraps Send)
        //

        #ifdef _STRING_

        inline Request & operator << (const std::string &S)
        {
            Send(S.c_str(), (int) S.length());
            return *this;
        }

        #endif

        //
        // Stream operator for sending an MFC CString (wraps Send)
        //

        #ifdef _AFX
        #ifdef UNICODE

        inline Request & operator << (const CString &S)
        {
            int Size = WideCharToMultiByte(CP_UTF8, 0, (const unsigned short *)S, -1, 0, 0, 0, 0);
            
            if(Size)
            {
                char * Buffer = new char[Size];
                WideCharToMultiByte(CP_UTF8, 0, (const unsigned short *)S, -1, Buffer, Size, 0, 0);
                Send(Buffer, Size - 1); // Don't include the null terminator
                delete [] Buffer;
            }

            return *this;
        }

        #else

        inline Request & operator << (const CString &S)
        {
            Send((const char *)S, S.GetLength());
            return *this;
        }

        #endif
        #endif

        //
        // If enabled, stream operators will send binary data instead of text. Disabled by default.
        //

        LacewingFunction void SetBinaryStream(bool Enabled);

        //
        // Returns the IP address of the request as an integer
        //

        LacewingFunction int IP();
 
        //
        // Returns the IP address of the request as a string
        //

        LacewingFunction const char * StringIP();


        //
        // Get the filename, MIME type or name (from the form) of file post data, for HandlerPreFilePost or HandlerFilePostChunk
        //

        const char * GetUploadFilename();
        const char * GetUploadMimeType();
        const char * GetUploadName();

        //
        // The URL of the request
        //
        
        const char * URL;

        //
        // The post data of the request (blank if the request is not a Post request)
        //

        const char * PostData;

        //
        // The hostname for the request
        //

        const char * Hostname;

        //
        // True if this request was secure (over HTTPS)
        //

        bool Secure;

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //
    
    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef void (__cdecl * HandlerGet)                  (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);
    typedef void (__cdecl * HandlerPreFilePost)          (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);
    typedef void (__cdecl * HandlerFilePostChunk)        (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request, const char * Buffer, int Size);
    typedef void (__cdecl * HandlerFilePostComplete)     (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);
    typedef void (__cdecl * HandlerPost)                 (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);
    typedef void (__cdecl * HandlerHead)                 (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);
    typedef void (__cdecl * HandlerError)                (Lacewing::Webserver &Webserver, Lacewing::Error &);

    //
    // These probably won't be required for normal use, but could be used to initialise/free something stored in the Request.Tag
    //

    typedef void (__cdecl * HandlerConnect)        (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request); 
    typedef void (__cdecl * HandlerDisconnect)     (Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerGet(HandlerGet, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPreFilePost(HandlerPreFilePost, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerFilePostChunk(HandlerFilePostChunk, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerFilePostComplete(HandlerFilePostComplete, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPost(HandlerPost, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerHead(HandlerHead, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class ProtocolExtension
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

public:

    virtual const char * GetName() = 0;


};

    
class Client
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

protected:

    //
    // The basic socket object
    //

    SocketClient Socket;

    //
    // The UDP object
    //

    UDP UDP;

public:

    //
    // Constructor and destructor
    //

    LacewingFunction Client();
    LacewingFunction ~Client();

    //
    // Connect to a server on the specified port
    //

    LacewingFunction void Connect(const char * Host, int Port = 6121, bool EnableUDP = true);

    //
    // Disconnect from the server. If Block is true, Disconnect() will not return until the disconnection is complete.
    //

    LacewingFunction void Disconnect(bool Block = true);

    //
    // Add or remove protocol extensions
    //

    LacewingFunction void AddProtocolExtension(Lacewing::ProtocolExtension &);
    LacewingFunction void RemoveProtocolExtension(Lacewing::ProtocolExtension &);

    //
    // Get the IP address of the server as a string
    //

    LacewingFunction const char * ServerIP();

    //
    // Get the server port the client is connected to
    //

    LacewingFunction int ServerPort();

    //
    // Determine the protocol implementation the server is using
    //

    LacewingFunction Lacewing::ProtocolImplementation::Type ServerImplementation();

    //
    // Get the client's unique ID on the server
    //

    LacewingFunction int GetID();

    //
    // Set the client's name
    //

    LacewingFunction void SetName(const char * Name);

    //
    // Get the client's name (blank if no name is set)
    //
    
    LacewingFunction const char * Name();

    //
    // Request a list of channels from the server
    //

    LacewingFunction void ListChannels();

    //
    // A channel on the server which has not been joined
    //

    struct ChannelListing
    {
        //
        // The number of peers in the channel
        //

        short PeerCount;

        //
        // The name of the channel
        //

        const char * Name;

    };

    //
    // Loop through all of the non-hidden channels on the server, after using ListChannels()
    //

    LacewingFunction int ChannelListLoop(int ID = 0);
    LacewingFunction ChannelListing * ChannelListLoopIndex(int ID);
    LacewingFunction void EndChannelListLoop(int ID);

    /*

        for(int ID = Client.ChannelListLoop(); ID; ID = Client.ChannelListLoop(ID))
        {
            Lacewing::Client::ChannelListing &Listing = Client.ChannelListLoopIndex(ID);            

            // If leaving the channel list loop before the end
            Channel.EndChannelListLoop();
        }

    */

    //
    // Request to join a channel. Hidden and AutoClose are only taken into account if you are joining
    // an empty channel (thus creating it and becoming the channel master)
    //

    LacewingFunction void Join(const char * Channel, bool Hidden = false, bool AutoClose = false);

    //
    // Send a message to the server
    //

    LacewingFunction void SendServer(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

    //
    // Blast a message to the server
    //

    LacewingFunction void BlastServer(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

    //
    // A structure to represent a file being sent to the server
    //

    struct File
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // The name of the file
        //

        const char * Filename;

        //
        // The percentage complete of the file transfer
        //

        int PercentageComplete();
    };

    //
    // Send a file to the server
    //

    LacewingFunction File * SendFile(int Subchannel, const char * Filename);

    //
    // Check if the client is connected
    //

    LacewingFunction bool Connected();

    //
    // Retrieve how long this client has been connected in seconds
    //

    LacewingFunction int ConnectionTime();

    //
    // Information regarding a joined channel
    //

    struct Channel
    {
        //
        // Internal tag (used inside the library)
        //
        
        void * InternalTag;

        //
        // True if you are the channel master
        //

        bool ChannelMaster;

        //
        // Get the channel name
        //

        LacewingFunction const char * Name();

        //
        // Send a message to the channel
        //

        LacewingFunction void Send(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);
        
        //
        // Blast a message to the channel
        //

        LacewingFunction void Blast(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

        //
        // A structure to represent a file being sent to the channel
        //

        struct File
        {
            //
            // Internal tag (used inside the library)
            //

            void * InternalTag;

            //
            // The name of the file
            //

            const char * Filename;

            //
            // The percentage complete of the file transfer
            //

            int PercentageComplete();

            //
            // Tag (use for whatever you want if you need to)
            //

            void * Tag;
        };

        //
        // Send a file to the channel
        //

        LacewingFunction File * SendFile(const char * Filename, int Subchannel= 0, __int64 Offset= 0, unsigned int Size = -1, const char * VirtualFilename="");

        //
        // Another client on a channel
        //

        struct Peer
        {
            //
            // Internal tag (used inside the library)
            //

            void * InternalTag;

            //
            // Unique numeric ID of the peer
            //

            int ID;

            //
            // True if this peer is the channel master
            //

            bool ChannelMaster;

            //
            // Send a message to the peer
            //

            LacewingFunction void Send(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

            //
            // Blast a message to the peer
            //

            LacewingFunction void Blast(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

            //
            // Get the peer name
            //

            LacewingFunction const char * Name();

            //
            // A structure to represent a file being sent to the peer
            //

            struct File
            {
            
                //
                // Internal tag (used inside the library)
                //

                void * InternalTag;

                //
                // The name of the file
                //

                const char * Filename;

                //
                // The percentage complete of the file transfer
                //
                
                int PercentageComplete();

                //
                // Tag (use for whatever you want if you need to)
                //

                void * Tag;
            };

            //
            // Send a file to the peer
            //

            LacewingFunction File * SendFile(int Subchannel, const char * Filename);

            //
            // Tag (use for whatever you want if you need to)
            //

            void * Tag;
        };

        //
        // Retrieve the number of peers in the channel
        //

        LacewingFunction int PeerCount();

        //
        // Loop through all the peers in the channel
        //

        LacewingFunction int PeerLoop(int ID = 0);
        LacewingFunction Peer &PeerLoopIndex(int ID);

        //
        // Prematurely end the peer loop
        //

        LacewingFunction void EndPeerLoop(int ID);

        /*

            for(int ID = Channel.PeerLoop(); ID; ID = Channel.PeerLoop(ID))
            {
                Lacewing::Client::Channel::Peer &Peer = Channel.PeerLoopIndex(ID);
                
                // If leaving the peer loop before the end
                Channel.EndPeerLoop(ID);
            }

        */

        //
        // Leave the channel
        //

        LacewingFunction void Leave();

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // The number of channels the client is in
    //

    LacewingFunction int ChannelCount();

    //
    // Loop through all the channels the client is connected to
    //

    LacewingFunction int ChannelLoop(int ID = 0);
    LacewingFunction Channel &ChannelLoopIndex(int ID);
    LacewingFunction void EndChannelLoop(int ID);

    /*

        for(int ID = Client.ChannelLoop(); ID; ID = Client.ChannelLoop(ID))
        {
            Lacewing::Client::Channel &Channel = Client.ChannelLoopIndex(ID);            
        
            // If leaving the client loop before the end
            Client.EndChannelLoop(ID);
        }

    */

    //
    // Find a channel which the client has already joined
    //

    LacewingFunction Channel * FindChannel(const char * Name);
    LacewingFunction Channel * FindChannel(unsigned int ID);

    //
    // Set the handler mode
    //

    LacewingFunction void SetHandlerMode(Lacewing::HandlerMode::Type Mode);
    
    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // Use this function when you only have one loop running ProcessHandlers() that you need to dispatch to
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef void    (__cdecl * HandlerConnect)                                  (Lacewing::Client &Client,const char * WelcomeMessage);
    typedef void    (__cdecl * HandlerConnectionDenied)                         (Lacewing::Client &Client,Lacewing::DenyStatus::Type Reason);
    typedef void    (__cdecl * HandlerDisconnect)                               (Lacewing::Client &Client);
    typedef void    (__cdecl * HandlerServerMessage)                            (Lacewing::Client &Client,Lacewing::TransmissionMode::Type Mode, int Subchannel,char * Buffer, int Size, int Type);
    typedef void    (__cdecl * HandlerChannelMessage)                           (Lacewing::Client &Client,Lacewing::Client::Channel &Channel,Lacewing::Client::Channel::Peer &Peer,Lacewing::TransmissionMode::Type Mode, int Subchannel,char * Buffer, int Size, int Type);
    typedef void    (__cdecl * HandlerPeerMessage)                              (Lacewing::Client &Client,Lacewing::Client::Channel &Channel,Lacewing::Client::Channel::Peer &Peer,Lacewing::TransmissionMode::Type Mode, int Subchannel,char * Buffer, int Size, int Type);
    typedef void    (__cdecl * HandlerError)                                    (Lacewing::Client &Client,Lacewing::Error &);
    typedef void    (__cdecl * HandlerJoin)                                     (Lacewing::Client &Client,Lacewing::Client::Channel &Target);
    typedef void    (__cdecl * HandlerJoinDenied)                               (Lacewing::Client &Client,Lacewing::DenyStatus::Type Reason);
    typedef void    (__cdecl * HandlerLeave)                                    (Lacewing::Client &Client,Lacewing::Client::Channel &Target);
    typedef void    (__cdecl * HandlerLeaveDenied)                              (Lacewing::Client &Client,Lacewing::Client::Channel &Target,Lacewing::DenyStatus::Type Reason);
    typedef void    (__cdecl * HandlerNameSet)                                  (Lacewing::Client &Client);
    typedef void    (__cdecl * HandlerNameChanged)                              (Lacewing::Client &Client,const char * OldName);
    typedef void    (__cdecl * HandlerNameDenied)                               (Lacewing::Client &Client,Lacewing::DenyStatus::Type Reason);
    typedef void    (__cdecl * HandlerPeerConnect)                              (Lacewing::Client &Client,Lacewing::Client::Channel &Channel,Lacewing::Client::Channel::Peer &Peer);
    typedef void    (__cdecl * HandlerPeerDisconnect)                           (Lacewing::Client &Client,Lacewing::Client::Channel &Channel,Lacewing::Client::Channel::Peer &Peer);
    typedef void    (__cdecl * HandlerPeerChangeName)                           (Lacewing::Client &Client,Lacewing::Client::Channel &Channel,Lacewing::Client::Channel::Peer &Peer,const char * OldName);
    typedef void    (__cdecl * HandlerChannelListComplete)                      (Lacewing::Client &Client);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerConnectionDenied(HandlerConnectionDenied, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerServerMessage(HandlerServerMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChannelMessage(HandlerChannelMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPeerMessage(HandlerPeerMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerJoin(HandlerJoin, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerJoinDenied(HandlerJoinDenied, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerLeave(HandlerLeave, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerLeaveDenied(HandlerLeaveDenied, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);    
    LacewingFunction void RegisterHandlerNameSet(HandlerNameSet, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerNameChanged(HandlerNameChanged, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerNameDenied(HandlerNameDenied, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPeerConnect(HandlerPeerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPeerDisconnect(HandlerPeerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPeerChangeName(HandlerPeerChangeName, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChannelListComplete(HandlerChannelListComplete, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class Server
{

private:

    //
    // Internal tag (used inside the library)
    //
    
    void * InternalTag;

protected:

    //
    // The basic socket object
    //

    SocketServer Socket;

    //
    // The UDP object
    //

    UDP UDP;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction Server();
    LacewingFunction ~Server();

    //
    // Host function
    //

    LacewingFunction void Host(int Port = 6121, bool EnableUDP = true);

    //
    // Stop hosting
    //

    LacewingFunction void Unhost();

    //
    // Check if the server is currently hosting
    //

    LacewingFunction bool Hosting();

    //
    // Get the port which the server is hosting on (0 if not hosting)
    //

    LacewingFunction int Port();

    //
    // Set the welcome message for the server
    //

    LacewingFunction void SetWelcomeMessage(const char * Message);

    //
    // Enable or disable channel listing (enabled by default)
    //

    LacewingFunction void SetChannelListing(bool Enabled);

    //
    // Set the maximum number of connections from a single IP address
    //

    LacewingFunction void SetMaximumConnectionsPerIP(int Maximum = 8);

    //
    // Set the amount (in ms) a client's flood timer should increase by with each connection
    //

    LacewingFunction void SetFloodGain(int Gain = 500);

    //
    // Set the value (in ms) a client's flood timer must be equal or greater than to deny a connection
    //

    LacewingFunction void SetFloodTolerance(int Tolerance = 1500);

    //
    // Set the value (in ms) a client's flood timer should increase by with each denied connection
    //

    LacewingFunction void SetFloodBlock(int Block = 10000);

    //
    // Add or remove protocol extensions
    //

    LacewingFunction void AddProtocolExtension(Lacewing::ProtocolExtension &);
    LacewingFunction void RemoveProtocolExtension(Lacewing::ProtocolExtension &);

    //
    // Retrieve the total number of bytes the server has sent and received
    //

    LacewingFunction __int64 BytesSent();
    LacewingFunction __int64 BytesReceived();

    //
    // The client structure so that the channel functions compile
    //

    struct Client;

    //
    // Structure to represent a channel on the server
    //

    struct Channel
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // Unique numeric ID of the channel
        //

        int ID;

        //
        // Get a pointer channel master (returns a null pointer if there isn't one)
        //

        LacewingFunction Client * GetChannelMaster();

        //
        // Get the name of the channel
        //

        LacewingFunction const char * Name();

        //
        // The number of clients in the channel
        //

        LacewingFunction int ClientCount();

        //
        // Loop through the clients in the channel
        //

        LacewingFunction int ClientLoop(int ID = 0);
        LacewingFunction Client &ClientLoopIndex(int ID);

        //
        // Prematurely end the client loop
        //

        LacewingFunction void EndClientLoop(int ID);

        /*

            for(int ID = Channel.ClientLoop(); ID; ID = Channel.ClientLoop(ID))
            {
                Lacewing::Server::Client &Client = Channel.ClientLoopIndex(ID);
                        
                // If leaving the client loop before the end
                Channel.EndClientLoop(ID);
            }

        */

        //
        // Returns true if the channel is hidden from the channel list
        //

        LacewingFunction bool Hidden();

        //
        // Returns true if the channel is set to close automatically
        //

        LacewingFunction bool AutoCloseEnabled();

        //
        // A structure to represent a file being sent to the server
        //

        struct File
        {
            //
            // The name of the file
            //

            const char * Filename;

            //
            // The percentage complete of the file transfer
            //

            int PercentageComplete();
        };

        //
        // Send a file to the channel
        //

        LacewingFunction File * SendFile(const char * Filename, int Subchannel= 0, __int64 Offset= 0, unsigned int Size = -1, const char * VirtualFilename="");

        //
        // Close the channel
        //

        LacewingFunction void Close();

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // The number of channels on the server
    //

    LacewingFunction int ChannelCount();

    //
    // Loop through all the channels
    //

    LacewingFunction int ChannelLoop(int ID = 0);
    LacewingFunction Channel &ChannelLoopIndex(int ID);
    LacewingFunction void EndChannelLoop(int ID);

    /*

        for(int ID = Server.ChannelLoop(); ID; ID = Server.ChannelLoop(ID))
        {
            Lacewing::Server::Channel &Channel = Server.ChannelLoopIndex(ID);
            
            // If leaving the channel loop before the end
            Server.EndChannelLoop(ID);
        }

    */

    //
    // Structure to represent a client on the server
    //

    struct Client
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // Numeric ID of the client
        //

        int ID;

        //
        // The address of the client as a 4 byte IP address
        //

        unsigned char IP[4];

        //
        // The port the server is connected to the client on
        //

        int Port;

        //
        // Convert the IP address to a string
        //

        LacewingFunction const char * StringIP();

        //
        // Determine the protocol implementation the client is using
        //

        LacewingFunction Lacewing::ProtocolImplementation::Type Implementation();

        //
        // Disconnect this client
        //

        LacewingFunction void Disconnect();

        //
        // Disconnect this client from within a client loop. The return value is a new loop ID - Index = Client.Disconnect(Index)
        //

        LacewingFunction int Disconnect(int LoopID);

        //
        // Retrieve how long this client has been connected in seconds
        //

        LacewingFunction int ConnectionTime();

        //
        // Retrieve the total number of bytes this client has been sent and the server has received from it
        //

        LacewingFunction __int64 BytesSent();
        LacewingFunction __int64 BytesReceived();

        //
        // Send a message to the client
        //

        LacewingFunction void Send(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

        //
        // Blast a message to the client
        //

        LacewingFunction void Blast(int Subchannel, const char * Buffer, int Size = -1, int Type = 0);

        //
        // Get the client name
        //

        LacewingFunction const char * Name();

        //
        // The number of channels the client is connected to
        //

        LacewingFunction int ChannelCount();

        //
        // Loop through all the channels the client is connected to
        //

        LacewingFunction int ChannelLoop(int ID = 0);
        LacewingFunction Channel &ChannelLoopIndex(int ID);

        //
        // Prematurely end the channel loop
        //

        LacewingFunction void EndChannelLoop(int ID);

        /*

            for(int ID = Client.ChannelLoop(); ID; ID = Client.ChannelLoop(ID))
            {
                Lacewing::Server::Channel &Channel = Client.ChannelLoopIndex(ID);
                
                // If leaving the channel loop before the end
                Server.EndChannelLoop(ID);
            }

        */

        //
        // A structure to represent a file being sent to the client
        //

        struct File
        {
            //
            // The name of the file
            //

            const char * Filename;

            //
            // The percentage complete of the file transfer
            //

            int PercentageComplete();
        };

        //
        // Send a file to the client
        //

        LacewingFunction File * SendFile(const char * Filename, int Subchannel= 0, __int64 Offset= 0, unsigned int Size = -1, const char * VirtualFilename="");

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // The number of clients on the server
    //

    LacewingFunction int ClientCount();

    //
    // Loop through the connected clients
    //

    LacewingFunction int ClientLoop(int ID = 0);
    LacewingFunction Client &ClientLoopIndex(int ID);

    //
    // Prematurely end the client loop
    //

    LacewingFunction void EndClientLoop(int ID);

    /*

        for(int ID = Server.ClientLoop(); ID; ID = Server.ClientLoop(ID))
        {
            Lacewing::Server::Client &Client = Server.ClientLoopIndex(ID);
            
            // If leaving the client loop before the end
            Server.EndClientLoop(ID);
        }

    */

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef Lacewing::Control::Type             (__cdecl * HandlerConnect)                                  (Lacewing::Server &Server, Lacewing::Server::Client &Client, Lacewing::DenyStatus::Type DenyStatus);
    typedef void                                (__cdecl * HandlerDisconnect)                               (Lacewing::Server &Server, Lacewing::Server::Client &Client);
    typedef void                                (__cdecl * HandlerError)                                    (Lacewing::Server &Server, Lacewing::Error &);
    typedef void                                (__cdecl * HandlerServerMessage)                            (Lacewing::Server &Server, Lacewing::Server::Client &Client, Lacewing::TransmissionMode::Type Mode, int Subchannel,char * Packet, int Size, int Type);
    typedef Lacewing::Control::Type             (__cdecl * HandlerChannelMessage)                           (Lacewing::Server &Server, Lacewing::Server::Client &Client, Lacewing::Server::Channel &Channel,Lacewing::TransmissionMode::Type Mode, int Subchannel,char * Packet, int Size, int Type);
    typedef Lacewing::Control::Type             (__cdecl * HandlerPeerMessage)                              (Lacewing::Server &Server, Lacewing::Server::Client &Client, Lacewing::Server::Channel &Channel,Lacewing::Server::Client &TargetClient,Lacewing::TransmissionMode::Type Mode, int Subchannel,char * Packet, int Size, int Type);
    typedef Lacewing::Control::Type             (__cdecl * HandlerJoinChannel)                              (Lacewing::Server &Server, Lacewing::Server::Client &Client, char * Name, Lacewing::DenyStatus::Type DenyStatus);
    typedef Lacewing::Control::Type             (__cdecl * HandlerLeaveChannel)                             (Lacewing::Server &Server, Lacewing::Server::Client &Client, Lacewing::Server::Channel &Channel, Lacewing::DenyStatus::Type DenyStatus);
    typedef Lacewing::Control::Type             (__cdecl * HandlerSetName)                                  (Lacewing::Server &Server, Lacewing::Server::Client &Client, char * Name, Lacewing::DenyStatus::Type DenyStatus);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerServerMessage(HandlerServerMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChannelMessage(HandlerChannelMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPeerMessage(HandlerPeerMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerJoinChannel(HandlerJoinChannel, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerLeaveChannel(HandlerLeaveChannel, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerSetName(HandlerSetName, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class Ping
{

private:

    //
    // Internal tag (used inside the library)
    //
    
    void * InternalTag;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction Ping();
    LacewingFunction ~Ping();

    //
    // Send one or more ping requests
    //

    LacewingFunction void SendPingRequest(const char * Hostname, int Timeout=250, int RequestCount=1);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);
    
    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);
    
    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef Lacewing::Control::Type         (__cdecl * HandlerPingSuccess)        (Lacewing::Ping &Ping, const char * Hostname, int RequestsLeft, int RTT, int TTL);
    typedef Lacewing::Control::Type         (__cdecl * HandlerPingFailure)        (Lacewing::Ping &Ping, const char * Hostname, int RequestsLeft);
    typedef void                            (__cdecl * HandlerError)              (Lacewing::Ping &Ping, Lacewing::Error &);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerPingSuccess(HandlerPingSuccess, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPingFailure(HandlerPingFailure, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class LAN
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

public:
    
    //
    // Constructor/destructor
    //

    LacewingFunction LAN();
    LacewingFunction ~LAN();

    //
    // Send one or more ping requests
    //

    LacewingFunction void EnumerateDevices();

    //
    // Structure to represent a device found on the LAN
    //

    struct Device
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // The address of the device as a 4 byte IP address
        //

        unsigned char IP[4];

        //
        // Returns the IP address as a string
        //

        LacewingFunction const char * StringIP();

        //
        // The hostname of the device
        //

        const char * Hostname;
    };

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef Lacewing::Control::Type            (__cdecl * HandlerFoundDevice)        (Lacewing::LAN &LAN,Lacewing::LAN::Device &Device);
    typedef void                               (__cdecl * HandlerError)              (Lacewing::LAN &LAN,Lacewing::Error &);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerFoundDevice(HandlerFoundDevice, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};

class FlashPlayerPolicy
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

protected:
    
    //
    // The socket server object
    //

    SocketServer Socket;

public:
    
    //
    // Constructor/destructor
    //

    LacewingFunction FlashPlayerPolicy();
    LacewingFunction ~FlashPlayerPolicy();

    //
    // Start hosting a policy XML file on a specified port
    //

    LacewingFunction void Host(const char * Filename, int Port = 843);

    //
    // Stop hosting the policy file
    //

    LacewingFunction void Unhost();

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef void (__cdecl * HandlerError) (Lacewing::FlashPlayerPolicy &FlashPlayerPolicy, Lacewing::Error &);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
};







//
// These ones aren't finished yet ;)
//

class IRC
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

protected:

    //
    // The basic socket object
    //

    SocketClient Socket;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction IRC();
    LacewingFunction ~IRC();

    //
    // Connect function
    //

    LacewingFunction void Connect(const char * Host, int Port, const char * Nick, const char * Username, const char * RealName, const char * Password = "", bool Invisible = false, bool Wallops = true);

    //
    // Disconnect function
    //

    LacewingFunction void Disconnect(const char * Reason = "Leaving");

    //
    // Check if the client is currently connected to a server
    //

    LacewingFunction bool Connected();

    //
    // Get the IP address of the server as a string
    //

    LacewingFunction const char * ServerIP();

    //
    // Get the server port the client is connected to
    //

    LacewingFunction int ServerPort();

    //
    // Set the nick
    //

    LacewingFunction void SetNick(const char * Nick);

    //
    // Join a channel
    //

    LacewingFunction void JoinChannel(const char * Channel, const char * Key = "");

    //
    // Structure to represent a fellow IRC user
    //

    struct Peer
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;
    
        //
        // Latest received peer information
        //

        const char * Nick;
        const char * User;
        const char * Host;

        //
        // Send a message to the peer
        //
        
        LacewingFunction void Send(const char * Message);

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // Structure to represent a channel on the server
    //

    struct Channel
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;
    
        //
        // Channel name
        //

        const char * Name;

        //
        // Channel topic
        //

        const char * Topic;

        //
        // Send a message to the channel
        //

        LacewingFunction void Send(const char * Message);

        //
        // Send an action (/me) to the channel
        //

        LacewingFunction void SendAction(const char * Action);

        //
        // Leave the channel
        //

        LacewingFunction void Leave(const char * Reason = "Leaving");

        //
        // Returns true if the specified peer has the specified mode on this channel
        // Mode could be o (operator), v (voice), etc
        //

        LacewingFunction bool PeerHasMode(Peer &Peer, char Mode);

        //
        // Retrieve the number of peers in the channel
        //

        LacewingFunction int PeerCount();

        //
        // Loop through all the peers in the channel
        //

        LacewingFunction int PeerLoop(int ID = 0);
        LacewingFunction Peer &PeerLoopIndex(int ID);

        //
        // Prematurely end the peer loop
        //

        LacewingFunction void EndPeerLoop(int ID);

        /*

            for(int ID = Channel.PeerLoop(); ID; ID = Channel.PeerLoop(ID))
            {
                Lacewing::IRC::Peer &Peer = Channel.PeerLoopIndex(ID);
                
                // If leaving the peer loop before the end
                Channel.EndPeerLoop(ID);
            }

        */

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // Set the handler mode
    //

    LacewingFunction void SetHandlerMode(Lacewing::HandlerMode::Type Mode);
    
    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef void    (__cdecl * HandlerConnect)                                    (Lacewing::IRC &IRC);
    typedef void    (__cdecl * HandlerDisconnect)                                 (Lacewing::IRC &IRC);
    typedef void    (__cdecl * HandlerError)                                      (Lacewing::IRC &IRC, Lacewing::Error &);
    typedef void    (__cdecl * HandlerNotice)                                     (Lacewing::IRC &IRC, const char * Notice);
    typedef void    (__cdecl * HandlerPeerMessage)                                (Lacewing::IRC &IRC, Lacewing::IRC::Peer &Peer, const char * Message);
    typedef void    (__cdecl * HandlerChannelMessage)                             (Lacewing::IRC &IRC, Lacewing::IRC::Channel &Channel, Lacewing::IRC::Peer &Peer, const char * Message);
    typedef void    (__cdecl * HandlerChannelJoined)                              (Lacewing::IRC &IRC, Lacewing::IRC::Channel &Channel);
    typedef void    (__cdecl * HandlerChannelLeft)                                (Lacewing::IRC &IRC, Lacewing::IRC::Channel &Channel);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerNotice(HandlerNotice, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChannelMessage(HandlerChannelMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPeerMessage(HandlerPeerMessage, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChannelJoined(HandlerChannelJoined, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChannelLeft(HandlerChannelLeft, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //
    void * Tag;
};

class FTPServer
{

private:

    //
    // Internal tag (used inside the library)
    //

    void * InternalTag;

protected:
    
    //
    // The raw socket objects used by the FTP server
    //

    Lacewing::SocketServer Control;
    Lacewing::SocketServer Data;

public:

    //
    // Constructor/destructor
    //

    LacewingFunction FTPServer();
    LacewingFunction ~FTPServer();

    //
    // Host an FTP server on the specified port
    //

    LacewingFunction void Host(int ControlPort = 21, int DataPort = 20);

    //
    // Stop hosting
    //

    LacewingFunction void Unhost();

    //
    // Check if the server is hosting
    //

    LacewingFunction bool Hosting();

    //
    // Get the port which the server is hosting on (0 if not hosting)
    //

    LacewingFunction int Port();

    //
    // Retrieve the total number of bytes the server has sent and received
    //

    LacewingFunction __int64 BytesSent();
    LacewingFunction __int64 BytesReceived();

    //
    // Structure to represent a client on the server
    //

    struct Client
    {
        //
        // Internal tag (used inside the library)
        //

        void * InternalTag;

        //
        // The address of the client as a 4 byte IP address
        //

        unsigned char IP[4];

        //
        // The port the server is connected to the client on
        //

        int Port;

        //
        // Returns the IP address of the client as a string
        //

        LacewingFunction const char * StringIP();

        //
        // Disconnect this client
        //

        LacewingFunction void Disconnect();

        //
        // Disconnect this client from within a client loop. The return value is a new loop ID - Index = Client->Disconnect(Index)
        //

        LacewingFunction int Disconnect(int LoopID);

        //
        // Retrieve how long this client has been connected in seconds
        //

        LacewingFunction int ConnectionTime();

        //
        // Retrieve the total number of bytes this client has been sent and the server has received from it
        //

        LacewingFunction __int64 BytesSent();
        LacewingFunction __int64 BytesReceived();

        //
        // Retrieve the username of the client
        //

        LacewingFunction const char * Username();

        //
        // Set the username as recognised (usually from a UsernameReceived handler)
        //

        LacewingFunction void SetUsernameRecognised(bool PasswordRequired);

        //
        // Set the current directory of this client
        //

        LacewingFunction void SetDirectory(const char * Directory);

        //
        // Tag (use for whatever you want if you need to)
        //

        void * Tag;
    };

    //
    // The number of clients connected to the server
    //

    LacewingFunction int ClientCount();

    //
    // Loop through the connected clients
    //

    LacewingFunction int ClientLoop(int ID = 0);
    LacewingFunction Client &ClientLoopIndex(int ID);
    
    //
    // Prematurely end the client loop
    //

    LacewingFunction void EndClientLoop(int ID);

    /*

        for(int ID = Server.ClientLoop(); ID; ID = Server.ClientLoop(ID))
        {
            Lacewing::SocketServer::Client &Client = Server.ClientLoopIndex(ID);
            
            // If leaving the client loop before the end
            Server.EndClientLoop(ID);
        }

    */

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    //

    LacewingFunction void ProcessHandlers();

    //
    // Use this function to add an ID that ProcessHandlers must be called with before the handler execution
    // will be marked as complete. This can be used if you have multiple loops calling ProcessHandlers()
    //

    LacewingFunction void AddHandlerProcessor(void * ProcessorID);

    //
    // Remove a handler processor added with AddHandlerProcessor
    //

    LacewingFunction void RemoveHandlerProcessor(void * ProcessorID);

    //
    // Trigger any waiting handlers (for Lacewing::HandlerMode::Queue or Lacewing::HandlerMode::Suspend)
    // and specify an ID (as passed to AddHandlerProcessor)
    //

    LacewingFunction void ProcessHandlers(void * ProcessorID);

    //
    // Available handlers for this class
    //

    typedef Lacewing::Control::Type    (__cdecl * HandlerConnect)                     (FTPServer &Server, FTPServer::Client &Client);
    typedef void                       (__cdecl * HandlerDisconnect)                  (FTPServer &Server, FTPServer::Client &Client);
    typedef void                       (__cdecl * HandlerUsernameReceived)            (FTPServer &Server, FTPServer::Client &Client);
    typedef Lacewing::Control::Type    (__cdecl * HandlerPasswordReceived)            (FTPServer &Server, FTPServer::Client &Client, const char * Password);
    typedef void                       (__cdecl * HandlerError)                       (FTPServer &Server, Lacewing::Error &);
    typedef Lacewing::Control::Type    (__cdecl * HandlerUnprocessedCommand)          (FTPServer &Server, FTPServer::Client &Client, const char * Command, const char * Arguments);
    typedef void                       (__cdecl * HandlerChangeDirectoryRequest)      (FTPServer &Server, FTPServer::Client &Client, const char * Directory);

    //
    // Register handlers
    //

    LacewingFunction void RegisterHandlerConnect(HandlerConnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerDisconnect(HandlerDisconnect, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerUsernameReceived(HandlerUsernameReceived, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerPasswordReceived(HandlerPasswordReceived, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerUnprocessedCommand(HandlerUnprocessedCommand, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerChangeDirectoryRequest(HandlerChangeDirectoryRequest, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);
    LacewingFunction void RegisterHandlerError(HandlerError, Lacewing::HandlerMode::Type Mode=Lacewing::HandlerMode::Trigger);

    //
    // Tag (use for whatever you want if you need to)
    //

    void * Tag;
};


// Lacewing namespace
};

// C++ check
#endif

// Include guard
#endif
