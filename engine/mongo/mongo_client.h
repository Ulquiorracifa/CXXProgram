#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <string>
#include <utils/platform.hpp>


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace std;

namespace engine::mongodb
{
    class EngineAPI mongo_client :std::enable_shared_from_this<mongo_client>
	{
        public:
            mongo_client();
            mongo_client(string hostName,string port);
            void setHostName(string hostName);
            void setPort(string port);
            void connectToHost();
            mongocxx::client* client();
            ~mongo_client();

            mongocxx::stdx::optional<bsoncxx::document::value> find_One(string db,string coll,document& filter,const mongocxx::v_noabi::options::find& options = mongocxx::v_noabi::options::find());
            mongocxx::cursor* find(string db,string coll,document& filter,const mongocxx::v_noabi::options::find& options = mongocxx::v_noabi::options::find());
            mongocxx::stdx::optional<mongocxx::result::insert_one> insert_One(string db,string coll,document& doc,const mongocxx::v_noabi::options::insert& options = {});
            mongocxx::stdx::optional<mongocxx::result::update> update_One(string db,string coll,document& filter,document& value,const mongocxx::v_noabi::options::update& options = mongocxx::v_noabi::options::update());
            mongocxx::stdx::optional<mongocxx::result::update> update_Many(string db,string coll,document& filter,document& value,const mongocxx::v_noabi::options::update& options = mongocxx::v_noabi::options::update());
            mongocxx::stdx::optional<mongocxx::result::delete_result> delete_One(string db,string coll,document& filter,const mongocxx::v_noabi::options::delete_options& options = mongocxx::v_noabi::options::delete_options());
            mongocxx::stdx::optional<mongocxx::result::delete_result> delete_Many(string db,string coll,document& filter,const mongocxx::v_noabi::options::delete_options& options = mongocxx::v_noabi::options::delete_options());
            std::int64_t countDocument(string db,string coll,document& filter,const mongocxx::options::count& option = mongocxx::options::count());
        protected:
            string m_hostName;
            string m_port;
        private:
            mongocxx::instance* m_dbInstance = nullptr;
            mongocxx::client* m_client = nullptr;
            mongocxx::uri uri;
            // static mongocxx::pool *s_pool = nullptr;
            unsigned int minPoolSize = 0;
            unsigned int maxPoolSize = 100;
	};
}