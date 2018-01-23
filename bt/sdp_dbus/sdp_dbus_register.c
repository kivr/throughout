#include <gdbus/gdbus.h>
#include <stdio.h>
#include <stdlib.h>

#define UUID "00001124-0000-1000-8000-00805f9b34fb"
#define error(args...) printf(args)

static DBusMessage * profile_release(DBusConnection *connection,
					DBusMessage *message, void *user_data)
{
	printf("Profile release\n");
	return NULL;
}

static DBusMessage * profile_new_connection(DBusConnection *connection,
					DBusMessage *message, void *user_data)
{
	printf("Profile new connection\n");
	return NULL;
}

static DBusMessage * profile_request_disconnection(
	DBusConnection *connection,	DBusMessage *message, void *user_data)
{
	printf("Profile request disconnection\n");
	return NULL;
}

static DBusMessage * profile_cancel(DBusConnection *connection,
					DBusMessage *message, void *user_data)
{
	printf("Profile cancel\n");
	return NULL;
}

static const GDBusMethodTable profile_methods[] = {
	{ GDBUS_METHOD("Release",
			NULL, NULL,
			profile_release) },
	{ GDBUS_METHOD("NewConnection",
			GDBUS_ARGS({"device", "o"}, {"fd", "h"},
			{"options", "a{sv}"}), NULL,
			profile_new_connection) },
	{ GDBUS_METHOD("RequestDisconnection",
			GDBUS_ARGS({"device", "o"}), NULL,
			profile_request_disconnection) },
	{ GDBUS_METHOD("Cancel",
			NULL, NULL,
			profile_cancel) },
	{ }
};

static char *readSDPRecord()
{
	char *data = NULL;
	FILE *sdp_record;
	size_t size = 0;
	
	if ((sdp_record = fopen("sdp_record.xml", "r")) != NULL)
	{
		fseek(sdp_record, 0L, SEEK_END);
		size = ftell(sdp_record);
		rewind(sdp_record);
		
		data = malloc(size);
		
		if (data != NULL)
		{
			fread(data, 1, size, sdp_record);
		}
		
		fclose(sdp_record);
	}
	
	return data;
}

static void register_profile_reply(DBusPendingCall *call, void *user_data)
{
	printf("Profile registered\n");
}

static void append_variant(DBusMessageIter *iter, int type, void *val)
{
	DBusMessageIter value;
	char sig[2] = { type, '\0' };

	dbus_message_iter_open_container(iter, DBUS_TYPE_VARIANT, sig, &value);

	dbus_message_iter_append_basic(&value, type, val);

	dbus_message_iter_close_container(iter, &value);
}

static void dict_append_entry(DBusMessageIter *dict,
			const char *key, int type, void *val)
{
	DBusMessageIter entry;

	if (type == DBUS_TYPE_STRING) {
		const char *str = *((const char **) val);
		if (str == NULL)
			return;
	}

	dbus_message_iter_open_container(dict, DBUS_TYPE_DICT_ENTRY,
							NULL, &entry);

	dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &key);

	append_variant(&entry, type, val);

	dbus_message_iter_close_container(dict, &entry);
}

static void unregister_profile(DBusConnection *connection, char *path)
{
	g_dbus_unregister_interface(connection, path,
						"org.bluez.Profile1");
	g_free(path);
}

static int register_profile(DBusConnection *connection)
{
	DBusMessage *msg;
	DBusMessageIter iter, opt;
	DBusPendingCall *call;
	dbus_bool_t requireAuthentication = FALSE;
	dbus_bool_t requireAuthorization = FALSE;
	char *role = "server";
	char *xml;
	int ret = 0;
	
	char *uuid = UUID;
	char *path = g_strconcat("/org/throughout/", UUID, NULL);
	g_strdelimit(path, "-", '_');

	if (!g_dbus_register_interface(connection, path,
					"org.bluez.Profile1", profile_methods,
					NULL, NULL,
					NULL /*data*/, NULL)) {
		error("D-Bus failed to register %s", path);
		g_free(path);
		path = NULL;
		return -1;
	}

	msg = dbus_message_new_method_call("org.bluez", "/org/bluez",
						"org.bluez.ProfileManager1",
						"RegisterProfile");

	dbus_message_iter_init_append(msg, &iter);

	dbus_message_iter_append_basic(&iter, DBUS_TYPE_OBJECT_PATH,
							&path);
	dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING,
							&uuid);
	dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY,
					DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
					DBUS_TYPE_STRING_AS_STRING
					DBUS_TYPE_VARIANT_AS_STRING
					DBUS_DICT_ENTRY_END_CHAR_AS_STRING,
					&opt);
	
	xml = readSDPRecord();
	
	dict_append_entry(&opt, "ServiceRecord", DBUS_TYPE_STRING,
								&xml);
	
	free(xml);
	
	dict_append_entry(&opt, "Role", DBUS_TYPE_STRING,
								&role);
	
	dict_append_entry(&opt, "RequireAuthentication", DBUS_TYPE_BOOLEAN,
								&requireAuthentication);
								
	dict_append_entry(&opt, "RequireAuthorization", DBUS_TYPE_BOOLEAN,
								&requireAuthorization);

	dbus_message_iter_close_container(&iter, &opt);

	if (!g_dbus_send_message_with_reply(connection, msg, &call, -1)) {
		ret = -1;
		unregister_profile(connection, path);
		goto failed;
	}

	dbus_pending_call_set_notify(call, register_profile_reply, NULL /*data*/,
									NULL);
	dbus_pending_call_unref(call);

failed:
	dbus_message_unref(msg);
	return ret;
}

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  DBusConnection *connection;
  DBusError derr;
  
  dbus_error_init(&derr);
  
  connection = g_dbus_setup_bus(DBUS_BUS_SYSTEM, "org.throughout", &derr);
  register_profile(connection);

  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  return 0;
}
