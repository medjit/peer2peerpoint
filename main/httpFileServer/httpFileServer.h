#ifndef HTTP_FILE_SERVER_H
#define HTTP_FILE_SERVER_H

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



//Tag used for logging
static const char *TAG_HTTP_FILE_SERVER = "HTTP_FILE_SERVER";

void httpFileServer_init(void);

const char headerHTMLChunk_1[] ="<!DOCTYPE html>"
                                "<html lang=\"en\">"
                                "<head>"
                                    "<style>"
                                        "*,"
                                        "*:before,"
                                        "*:after {"
                                            "box-sizing: inherit;"
                                            "text-decoration: none;"
                                            "color: inherit;"
                                        "}"
                                        ".u-clearfix:before,"
                                        ".u-clearfix:after {"
                                            "content: "";"
                                            "display: table;"
                                        "}"
                                        ".u-clearfix:after {"
                                            "clear: both;"
                                        "}"
                                        ""
                                        ".recipeWrapper {"
                                            "display: flex;"
                                            "flex-wrap: wrap;"
                                            "row-gap: 10px;"
                                            "column-gap: 2em;"
                                        "}"
                                        ".recipeWrapper > * {"
                                            "flex: 1;"
                                        "}"
                                        ".subtle {"
                                            "color: #aaa;"
                                        "}"
                                        ".card-container {"
                                            "margin: 25px auto 0;"
                                            "width: 600px;"
                                            "min-width: 580px;"
                                            "max-width: 620px;"
                                            "display: flex;"
                                            "flex-wrap: wrap;"
                                            "row-gap: 10px;"
                                            "column-gap: 2em;"
                                            "border: 1px solid #aaa;"
                                        "}"
                                        ".card-container > * {"
                                            "flex: 1;"
                                        "}"
                                        ".card {"
                                            "background-color: #fff;"
                                            "padding: 30px;"
                                            "position: relative;"
                                            "z-index: 1;"
                                        "}"
                                        ".card-body {"
                                            "width: 250px;"
                                            "float: left;"
                                        "}"
                                        ".card-author {"
                                            "font-size: 12px;"
                                            "letter-spacing: 0.5px;"
                                            "margin: 15px 0 0;"
                                            "text-transform: uppercase;"
                                        "}"
                                        ".categoryWrap {"
                                            "display: flex;"
                                            "justify-content: space-between;"
                                            "padding: 0;"
                                            "margin: 0;"
                                        "}"
                                        ".card-category {"
                                            "font-size: 12px;"
                                            "letter-spacing: 0.5px;"
                                            "margin: 15px 0 0;"
                                            "text-transform: uppercase;"
                                            "text-align: right;"
                                        "}"
                                        ".card-title {"
                                            "font-size: 38px;"
                                            "font-weight: 300;"
                                            "line-height: 60px;"
                                            "margin: 20px 0;"
                                            "color: #3e4960;"
                                            "text-transform: upper;"
                                        "}"
                                        ".card-description {"
                                            "display: inline-block;"
                                            "font-weight: 300;"
                                            "line-height: 22px;"
                                            "margin: 10px 0;"
                                            "font-size: 16px;"
                                            "text-align: justify;"
                                            "text-justify: inter-word;"
                                        "}"
                                        ".card-read {"
                                            "cursor: pointer;"
                                            "font-size: 12pt;"
                                            "font-weight: 700;"
                                            "letter-spacing: 6px;"
                                            "margin: 5px 0 20px;"
                                            "position: relative;"
                                            "text-align: right;"
                                            "text-transform: uppercase;"
                                        "}"
                                        ".card-read:hover {"
                                            "font-size: 12.5pt;"
                                            "color: #586683;"
                                        "}"
                                        ".card-read:after {"
                                            "background-color: #b8bddd;"
                                            "content: "";"
                                            "display: block;"
                                            "height: 1px;"
                                            "position: absolute;"
                                            "top: 9px;"
                                            "width: 40;"
                                        "}"
                                        ".card-media {"
                                            "float: right;"
                                            "width: 240px;"
                                            "height: 360px;"
                                            "margin: 0;"
                                            "padding: 0;"
                                            "object-fit: cover;"
                                        "}"
                                        /* Base styles for navibar*/
                                        ".navbar {"
                                            "display: flex;"
                                            "justify-content: space-around;"
                                            "background-color: #333;"
                                        "}"

                                        ".navbar a {"
                                            "color: white;"
                                            "padding: 14px 20px;"
                                            "text-decoration: none;"
                                            "text-align: center;"
                                        "}"

                                        ".navbar a:hover {"
                                            "background-color: #ddd;"
                                            "color: black;"
                                        "}"
                                        /* Responsive styles */
                                        "@media (max-width: 600px) {"
                                            ".navbar {"
                                                "flex-direction: column;"
                                            "}"

                                            ".navbar a {"
                                                "padding: 10px;"
                                                "border-top: 1px solid #ddd;"
                                            "}"

                                            ".navbar a:first-child {"
                                                "border-top: none;"
                                            "}"
                                        "}";
                                        /* footer cssL */
 const char headerHTMLChunk_2[] =       ".content {"
                                            "flex: 1;"
                                            "padding: 20px;"
                                        "}"
                                        ".container {"
                                            "background-color: #f4f4f9;"
                                            "display: grid;"
                                            "gap: 10px;"
                                            "padding: 10px;"
                                            "grid-template-columns: repeat(auto-fit, minmax(350px, 1fr));"
                                        "}"
                                        ".footer {"
                                            "display: flex;"
                                            "justify-content: space-between;"
                                            "align-items: center;"
                                            "padding: 10px;"
                                            "background-color: #f1f1f1;"
                                            "border-top: 1px solid #ccc;"
                                            "font-size: 14px;"
                                        "}"

                                        ".battery-icon {"
                                            "width: 24px;"
                                            "height: 24px;"
                                            "background-image: url('battery-icon.png');"
                                            "background-size: cover;"
                                            "margin-right: 5px;"
                                        "}"

                                        ".system-info-body {"
                                            "font-family: Arial, sans-serif;"
                                            "display: flex;"
                                            "justify-content: center;"
                                            "align-items: center;"
                                            "height: 100vh;"
                                            "margin: 0;"
                                            "background-color: #f4f4f9;"
                                        "}"

                                        ".system-info {"
                                            "border: 1px solid #ccc;"
                                            "border-radius: 10px;"
                                            "padding: 20px;"
                                            "margin: 5px;"
                                            "background-color: #fff;"
                                            "box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
                                            "text-align: center;"
                                            "max-width: 400px;"
                                            "width: 80;"
                                        "}"

                                        ".system-info h2 {"
                                            "margin-top: 0;"
                                            "color: #333;"
                                        "}"

                                        ".info-item {"
                                            "margin-bottom: 15px;"
                                        "}"

                                        ".info-item span {"
                                            "font-weight: bold;"
                                            "display: block;"
                                            "color: #555;"
                                        "}"

                                        ".info-value {"
                                            "font-weight: normal;"
                                            "color: #000;"
                                        "}"
                                    "</style>"
                                    "<meta charset=\"UTF-8\">"
                                    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> "
                                    "<title> "
                                        "Peer2PeerPoint"
                                    "</title>"
                                "</head>"
                                "<body>"
                                "<div class=\"navbar\">"
                                    "<a href=\"/\">Начало</a>"
                                    "<a href=\"/files\">Файлове</a>"
                                    "<a href=\"/systemstatus\">Статус</a>"
                                    "<a href=\"loginout\">Потребител</a>"
                                "</div>";

const char footerHTML[] = "<div class=\"footer\">"
                            "<div>"
                                "<span class=\"battery-icon\"></span>"
                                "<span>Battery Voltage: 3.7V</span>"
                            "</div>"
                            "<div>"
                                "<span id=\"dateTime\"></span>"
                            "</div>"
                            "<div>"
                                "<span>Username: johndoe</span>"
                            "</div>"
                        "</div>"
                        "<script>"
                            "function updateDateTime() {"
                            "const now = new Date();"
                            "const formattedDateTime = now.toLocaleDateString() + ' ' + now.toLocaleTimeString();"
                            "document.getElementById('dateTime').textContent = formattedDateTime;"
                            "}"
                            "setInterval(updateDateTime, 1000); // Update the date and time every second updateDateTime(); // Initial call to set the date and time immediately "
                        "</script>"
                    "</body>"
                "</html>";


esp_err_t httpFileServer_sendFileChunkHTML(httpd_req_t *req, char *author, char *title, char *theme, char *download_address)
{
// --------------------------- Card Template ---------------------------
    const char cardTemplateChunk_1[] =  "<div class=\"card-container\">"
                                            "<div class=\"card u-clearfix\">"
                                                "<div class=\"card-body\">"
                                                    "<p class=\"categoryWrap\">"
                                                        "<span class=\"card-author subtle\">";
                                                            /* author */
    const char cardTemplateChunk_2[] =                  "</span>"
                                                    "</p>"
                                                    "<h2 id=\"title\" onload=\"trimTitle()\" class=\"card-title\">";
                                                        /* Long tite */
    const char cardTemplateChunk_3[] =              "</h2>"
                                                    "<span class=\"card-description subtle\">";
                                                        /* Description / theme */
    const char cardTemplateChunk_4[] =              "</span>"
                                                "<div class=\"card-read\">"
                                                    "<a href=\"";
                                                        /* Downloading address*/
    const char cardTemplateChunk_5[] =              "Download</a>"
                                                "</div>"
                                            "</div>"
                                            "<img src=\"\" alt=\"{{ recipe.title }}\" class=\"card-media\" />"
                                            "</div>"
                                        "</div>";

    httpd_resp_send_chunk(req, cardTemplateChunk_1, sizeof(cardTemplateChunk_1));
    httpd_resp_send_chunk(req, author, sizeof(author));
    httpd_resp_send_chunk(req, cardTemplateChunk_2, sizeof(cardTemplateChunk_2));
    httpd_resp_send_chunk(req, title, sizeof(title));
    httpd_resp_send_chunk(req, cardTemplateChunk_3, sizeof(cardTemplateChunk_3));
    httpd_resp_send_chunk(req, theme, sizeof(theme));
    httpd_resp_send_chunk(req, cardTemplateChunk_4, sizeof(cardTemplateChunk_4));
    httpd_resp_send_chunk(req, download_address, sizeof(download_address));
    httpd_resp_send_chunk(req, cardTemplateChunk_5, sizeof(cardTemplateChunk_5));
    return ESP_OK;
}


esp_err_t get_home_handler(httpd_req_t *req)
{
    httpd_resp_send_chunk(req, headerHTMLChunk_1, sizeof(headerHTMLChunk_1));
    httpd_resp_send_chunk(req, headerHTMLChunk_2, sizeof(headerHTMLChunk_2));
    httpd_resp_send_chunk(req, "<div class=\"container\">", sizeof("<div class=\"container\">"));
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpFileServer_sendFileChunkHTML(req, "Test author", "This is veeeeery long title for thest", "This is theme of the content", "This is download address for test");
    httpd_resp_send_chunk(req, "</div>", sizeof("</div>"));
    httpd_resp_send_chunk(req, footerHTML, sizeof(footerHTML));
    //httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

esp_err_t get_files_handler(httpd_req_t *req)
{
    httpd_resp_send_chunk(req, headerHTMLChunk_1, sizeof(headerHTMLChunk_1));
    httpd_resp_send_chunk(req, headerHTMLChunk_2, sizeof(headerHTMLChunk_2));
    httpd_resp_send_chunk(req, "<div class=\"container\">", sizeof("<div class=\"container\">"));
        httpd_resp_send_chunk(req, "Text instead of cards", sizeof("Text instead of cards"));
    httpd_resp_send_chunk(req, "</div>", sizeof("</div>"));
    httpd_resp_send_chunk(req, footerHTML, sizeof(footerHTML));
    //httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

esp_err_t not_found_handler(httpd_req_t *req)
{
    const char resp[] = "<!DOCTYPE html>"
                        "<html lang=\"en\">"
                        "<head>"
                            "<style>"
                                "* {"
                                    "margin: 0;"
                                    "padding: 0;"
                                    "box-sizing: border-box;"
                                "}"
                                "body {"
                                    "font-family: Arial, sans-serif;"
                                    "background-color: #D3D3D3;"
                                    "display: flex;"
                                    "justify-content: center;"
                                    "align-items: center;"
                                    "height: 100vh;"
                                "}"
                                ".error-container {"
                                    "text-align: center;"
                                    "background-color: #fff;"
                                    "padding: 20px;"
                                    "border-radius: 5px;"
                                    "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"
                                "}"
                                "h1 {"
                                    "font-size: 5rem;"
                                    "color: #ff5733;"
                                "}"
                                "p {"
                                    "font-size: 1.5rem;"
                                    "color: #333;"
                                    "margin-bottom: 20px;"
                                "}"
                                "a {"
                                    "text-decoration: none;"
                                    "background-color: #ff5733;"
                                    "color: #fff;"
                                    "padding: 10px 20px;"
                                    "border-radius: 3px;"
                                    "font-weight: bold;"
                                    "transition: background-color 0.3s ease;"
                                "}"
                                "a:hover {"
                                    "background-color: #e6482e;"
                                "}"
                            "</style>"
                            "<meta charset=\"UTF-8\">"
                            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> "
                            "<title> "
                                "404 Страницата не е намерена"
                            "</title>"
                        "</head>"
                        "<body>"
                            "<div class=\"error-container\">"
                                "<h1> 404 </h1>"
                                "<p>Страницата не е намерена</p>"
                                "<a href=\"/\">"
                                    "Към началната страница"
                                "</a>"
                            "</div>"
                        "</body>"
                        "</html>";

    // Set the status code to 404
    httpd_resp_set_status(req, "404 Not Found");

    // Send the HTML response
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static const httpd_uri_t get_home = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = get_home_handler,
    .user_ctx = NULL
};

static const httpd_uri_t get_files = {
    .uri       = "/files",
    .method    = HTTP_GET,
    .handler   = get_files_handler,
    .user_ctx = NULL
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Adjust other configuration parameters if necessary
    // For example, increasing the task stack size:
    config.stack_size = 8192;

    // Start the httpd server
    ESP_LOGI(TAG_HTTP_FILE_SERVER, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG_HTTP_FILE_SERVER, "Registering URI handlers");
        httpd_register_uri_handler(server, &get_home);
        httpd_register_uri_handler(server, &get_files);
        
        // Set the global default handler for 404 not found
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, not_found_handler);
        return server;
    }

    ESP_LOGI(TAG_HTTP_FILE_SERVER, "Error starting server!");
    return NULL;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* Stop the httpd server */
        httpd_stop(server);
        return ESP_OK;
    }
    return ESP_FAIL;
}

void httpFileServer_init(void)
{
    ESP_LOGI(TAG_HTTP_FILE_SERVER, "Configure http file server.");
    static httpd_handle_t server = NULL;

    /* Start the server for the first time */
    server = start_webserver();
}

#endif /* HTTP_FILE_SERVER_H */