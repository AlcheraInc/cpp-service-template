
#include <cstdlib>
#include <grpcpp/grpcpp.h>
#include <service.grpc.pb.h>

#include "loader.h"

void serve(grpc::Server& launcher, //
           grpc_impl::ServerCompletionQueue& queue,
           svc_v1::SH::AsyncService& service) noexcept(false);

// "service.exe" "0.0.0.0:9090" "service.pem" "service.key"
int main(int argc, char* argv[]) try {
    grpc::SslServerCredentialsOptions tls{};
    grpc::ServerBuilder builder{};

    auto creds = grpc::InsecureServerCredentials();
    if (argc < 2) {
        puts("expects address in argv[1] \n");
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        puts("using TLS \n");
        tls.pem_key_cert_pairs.push_back({argv[4], argv[3]});
        creds = grpc::SslServerCredentials(tls);
    }
    builder.AddListeningPort(argv[2], creds);

    svc_v1::SH::AsyncService as{};
    builder.RegisterService(&as);

    auto queue = builder.AddCompletionQueue();
    auto launcher = builder.BuildAndStart();
    serve(*launcher, *queue, as);
    return EXIT_SUCCESS;

} catch (const std::error_code& ec) {
    auto msg = ec.message();
    fputs(msg.c_str(), stderr);
    return ec.value();

} catch (const std::system_error& ex) {
    fputs(ex.what(), stderr);
    return ex.code().value();

} catch (const std::exception& ex) {
    fputs(ex.what(), stderr);
    return EXIT_FAILURE;
}
