#ifndef HTTP_FILE_SERVER_H
#define HTTP_FILE_SERVER_H

#include "../webUiRender/webUiRender.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_check.h"

#if !CONFIG_IDF_TARGET_LINUX
#include <esp_wifi.h>
#include <esp_system.h>
#include "nvs_flash.h"
#include "esp_eth.h"
#endif  // !CONFIG_IDF_TARGET_LINUX

#define EXAMPLE_HTTP_QUERY_KEY_MAX_LEN  (64)



//Tag used for logging
static const char *TAG_HTTP_FILE_SERVER = "HTTP_FILE_SERVER";

void httpFileServer_init(void);






/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG_HTTP_FILE_SERVER, "buffer alloc failed");
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG_HTTP_FILE_SERVER, "buffer alloc failed");
        if (httpd_req_get_hdr_value_str(req, "Test-Header-2", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found header => Test-Header-2: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG_HTTP_FILE_SERVER, "buffer alloc failed");
        if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found header => Test-Header-1: %s", buf);
        }
        free(buf);
    }

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG_HTTP_FILE_SERVER, "buffer alloc failed");
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found URL query => %s", buf);
            char param[EXAMPLE_HTTP_QUERY_KEY_MAX_LEN], dec_param[EXAMPLE_HTTP_QUERY_KEY_MAX_LEN] = {0};
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found URL query parameter => query1=%s", param);
                //example_uri_decode(dec_param, param, strnlen(param, EXAMPLE_HTTP_QUERY_KEY_MAX_LEN));
                ESP_LOGI(TAG_HTTP_FILE_SERVER, "Decoded query parameter => %s", dec_param);
            }
            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found URL query parameter => query3=%s", param);
                //example_uri_decode(dec_param, param, strnlen(param, EXAMPLE_HTTP_QUERY_KEY_MAX_LEN));
                ESP_LOGI(TAG_HTTP_FILE_SERVER, "Decoded query parameter => %s", dec_param);
            }
            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG_HTTP_FILE_SERVER, "Found URL query parameter => query2=%s", param);
                //example_uri_decode(dec_param, param, strnlen(param, EXAMPLE_HTTP_QUERY_KEY_MAX_LEN));
                ESP_LOGI(TAG_HTTP_FILE_SERVER, "Decoded query parameter => %s", dec_param);
            }
        }
        free(buf);
    }

    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG_HTTP_FILE_SERVER, "Request headers lost");
    }
    return ESP_OK;
}

/* Our URI handler function to be called during GET /uri request */
esp_err_t get_home_handler(httpd_req_t *req)
{
    /* Send a simple response */
    const char resp[] = "This is home page";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "Hello World!"
};

static const httpd_uri_t get_home = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = get_home_handler,
    .user_ctx = NULL
};


/* This handler allows the custom error handling functionality to be
 * tested from client side. For that, when a PUT request 0 is sent to
 * URI /ctrl, the /hello and /echo URIs are unregistered and following
 * custom error handler http_404_error_handler() is registered.
 * Afterwards, when /hello or /echo is requested, this custom error
 * handler is invoked which, after sending an error message to client,
 * either closes the underlying socket (when requested URI is /echo)
 * or keeps it open (when requested URI is /hello). This allows the
 * client to infer if the custom error handler is functioning as expected
 * by observing the socket state.
 */
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } else if (strcmp("/echo", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
#if CONFIG_IDF_TARGET_LINUX
    // Setting port as 8001 when building for Linux. Port 80 can be used only by a privileged user in linux.
    // So when a unprivileged user tries to run the application, it throws bind error and the server is not started.
    // Port 8001 can be used by an unprivileged user as well. So the application will not throw bind error and the
    // server will be started.
    config.server_port = 8001;
#endif // !CONFIG_IDF_TARGET_LINUX
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG_HTTP_FILE_SERVER, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG_HTTP_FILE_SERVER, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        httpd_register_uri_handler(server, &get_home);
        #if CONFIG_EXAMPLE_BASIC_AUTH
        httpd_register_basic_auth(server);
        #endif
        return server;
    }

    ESP_LOGI(TAG_HTTP_FILE_SERVER, "Error starting server!");
    return NULL;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    return httpd_stop(server);
}

void httpFileServer_init(void)
{
    ESP_LOGI(TAG_HTTP_FILE_SERVER, "Configure http file server.");
    static httpd_handle_t server = NULL;


    /* Start the server for the first time */
    server = start_webserver();
}

#endif /* HTTP_FILE_SERVER_H */