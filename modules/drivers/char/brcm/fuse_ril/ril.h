/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /* ISSUES:
 * - SMS retransmit (specifying TP-Message-ID)
 *
 */

/**
 * TODO
 *
 * Supp Service Notification (+CSSN)
 * GPRS PDP context deactivate notification
 *
 */


#ifndef ANDROID_RIL_H
#define ANDROID_RIL_H 1

//#include <stdlib.h>
#ifndef FEATURE_UNIT_TEST
//#include <sys/time.h>
#endif /* !FEATURE_UNIT_TEST */

#ifdef __cplusplus
extern "C" {
#endif

#define RIL_VERSION 3

#define CDMA_ALPHA_INFO_BUFFER_LENGTH 64
#define CDMA_NUMBER_INFO_BUFFER_LENGTH 81

/**
 * Configuration Flag
 * Refer to /secril_multi/config.h
 * If you want to undefine this flag, you must undefine same flag in config_MODEL.h
*/
#define CONFIG_VT_SUPPORT


typedef void * RIL_Token;

typedef enum {
    RIL_E_SUCCESS = 0,
    RIL_E_RADIO_NOT_AVAILABLE = 1,     /* If radio did not start or is resetting */
    RIL_E_GENERIC_FAILURE = 2,
    RIL_E_PASSWORD_INCORRECT = 3,      /* for PIN/PIN2 methods only! */
    RIL_E_SIM_PIN2 = 4,                /* Operation requires SIM PIN2 to be entered */
    RIL_E_SIM_PUK2 = 5,                /* Operation requires SIM PIN2 to be entered */
    RIL_E_REQUEST_NOT_SUPPORTED = 6,
    RIL_E_CANCELLED = 7,
    RIL_E_OP_NOT_ALLOWED_DURING_VOICE_CALL = 8, /* data ops are not allowed during voice
                                                   call on a Class C GPRS device */
    RIL_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW = 9,  /* data ops are not allowed before device
                                                   registers in network */
    RIL_E_SMS_SEND_FAIL_RETRY = 10,             /* fail to send sms and need retry */
    RIL_E_SIM_ABSENT = 11,                      /* fail to set the location where CDMA subscription
                                                   shall be retrieved because of SIM or RUIM
                                                   card absent */
    RIL_E_SUBSCRIPTION_NOT_AVAILABLE = 12,      /* fail to find CDMA subscription from specified
                                                   location */
    RIL_E_MODE_NOT_SUPPORTED = 13,              /* HW does not support preferred network type */
    RIL_E_FDN_CHECK_FAILURE = 14,               /* command failed because recipient is not on FDN list */
    RIL_E_ILLEGAL_SIM_OR_ME = 15                /* network selection failed due to
                                                   illegal SIM or ME */
} RIL_Errno;

typedef enum {
    RIL_CALL_ACTIVE = 0,
    RIL_CALL_HOLDING = 1,
    RIL_CALL_DIALING = 2,    /* MO call only */
    RIL_CALL_ALERTING = 3,   /* MO call only */
    RIL_CALL_INCOMING = 4,   /* MT call only */
    RIL_CALL_WAITING = 5     /* MT call only */
} RIL_CallState;

typedef enum {
    RADIO_STATE_OFF = 0,                   /* Radio explictly powered off (eg CFUN=0) */
    RADIO_STATE_UNAVAILABLE = 1,           /* Radio unavailable (eg, resetting or not booted) */
    RADIO_STATE_SIM_NOT_READY = 2,         /* Radio is on, but the SIM interface is not ready */
    RADIO_STATE_SIM_LOCKED_OR_ABSENT = 3,  /* SIM PIN locked, PUK required, network
                                              personalization locked, or SIM absent */
    RADIO_STATE_SIM_READY = 4,             /* Radio is on and SIM interface is available */
    RADIO_STATE_RUIM_NOT_READY = 5,        /* Radio is on, but the RUIM interface is not ready */
    RADIO_STATE_RUIM_READY = 6,            /* Radio is on and the RUIM interface is available */
    RADIO_STATE_RUIM_LOCKED_OR_ABSENT = 7, /* RUIM PIN locked, PUK required, network
                                              personalization locked, or RUIM absent */
    RADIO_STATE_NV_NOT_READY = 8,          /* Radio is on, but the NV interface is not available */
    RADIO_STATE_NV_READY = 9               /* Radio is on and the NV interface is available */
} RIL_RadioState;



typedef struct {
  RIL_RadioState   radioState ;    /* radioState*/
  int             simCardType;  /* simCardType */
  int             IsFlightModeOnBoot;  // gearn flihgt mode simcard type
} RIL_SIM_Chaged;  // gearn sim card type

/* User-to-User signaling Info activation types derived from 3GPP 23.087 v8.0 */
typedef enum {
    RIL_UUS_TYPE1_IMPLICIT = 0,
    RIL_UUS_TYPE1_REQUIRED = 1,
    RIL_UUS_TYPE1_NOT_REQUIRED = 2,
    RIL_UUS_TYPE2_REQUIRED = 3,
    RIL_UUS_TYPE2_NOT_REQUIRED = 4,
    RIL_UUS_TYPE3_REQUIRED = 5,
    RIL_UUS_TYPE3_NOT_REQUIRED = 6
} RIL_UUS_Type;

/* User-to-User Signaling Information data coding schemes. Possible values for
 * Octet 3 (Protocol Discriminator field) in the UUIE. The values have been
 * specified in section 10.5.4.25 of 3GPP TS 24.008 */
typedef enum {
    RIL_UUS_DCS_USP = 0,          /* User specified protocol */
    RIL_UUS_DCS_OSIHLP = 1,       /* OSI higher layer protocol */
    RIL_UUS_DCS_X244 = 2,         /* X.244 */
    RIL_UUS_DCS_RMCF = 3,         /* Reserved for system mangement
                                     convergence function */
    RIL_UUS_DCS_IA5c = 4          /* IA5 characters */
} RIL_UUS_DCS;

/* User-to-User Signaling Information defined in 3GPP 23.087 v8.0
 * This data is passed in RIL_ExtensionRecord and rec contains this
 * structure when type is RIL_UUS_INFO_EXT_REC */
typedef struct {
  RIL_UUS_Type    uusType;    /* UUS Type */
  RIL_UUS_DCS     uusDcs;     /* UUS Data Coding Scheme */
  int             uusLength;  /* Length of UUS Data */
  char *          uusData;    /* UUS Data */
} RIL_UUS_Info;

/* CDMA Signal Information Record as defined in C.S0005 section 3.7.5.5 */
typedef struct {
  char isPresent;    /* non-zero if signal information record is present */
  char signalType;   /* as defined 3.7.5.5-1 */
  char alertPitch;   /* as defined 3.7.5.5-2 */
  char signal;       /* as defined 3.7.5.5-3, 3.7.5.5-4 or 3.7.5.5-5 */
} RIL_CDMA_SignalInfoRecord;

typedef struct {
    RIL_CallState   state;
    int             index;      /* Connection Index for use with, eg, AT+CHLD */
    int             toa;        /* type of address, eg 145 = intl */
    char            isMpty;     /* nonzero if is mpty call */
    char            isMT;       /* nonzero if call is mobile terminated */
    char            als;        /* ALS line indicator if available
                                   (0 = line 1) */
    char            isVoice;    /* nonzero if this is is a voice call */
#ifdef CONFIG_VT_SUPPORT
	//ADDED  BY SISO ON 2nd April
    char            isVideo;    /* nonzero if this is is a Video call */
	//FINISHED SISO UPDATION 2nd April
#endif
    char            isVoicePrivacy;     /* nonzero if CDMA voice privacy mode is active */
    char *          number;     /* Remote party number */
    int             numberPresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown 3=Payphone */
    char *          name;       /* Remote party name */
    int             namePresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown 3=Payphone */
    RIL_UUS_Info *  uusInfo;    /* NULL or Pointer to User-User Signaling Information */
} RIL_Call;

typedef struct {
    int             cid;        /* Context ID */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char *          type;       /* X.25, IP, IPV6, etc. */
    char *          apn;
    char *          address;
} RIL_Data_Call_Response;

typedef struct {
    int messageRef;   /* TP-Message-Reference for GSM,
                         and BearerData MessageId for CDMA
                         (See 3GPP2 C.S0015-B, v2.0, table 4.5-1). */
    char *ackPDU;     /* or NULL if n/a */
    int errorCode;    /* See 3GPP 27.005, 3.2.5 for GSM/UMTS,
                         3GPP2 N.S0005 (IS-41C) Table 171 for CDMA,
                         -1 if unknown or not applicable*/
} RIL_SMS_Response;

/** Used by RIL_REQUEST_WRITE_SMS_TO_SIM */
typedef struct {
    int status;     /* Status of message.  See TS 27.005 3.1, "<stat>": */
                    /*      0 = "REC UNREAD"    */
                    /*      1 = "REC READ"      */
                    /*      2 = "STO UNSENT"    */
                    /*      3 = "STO SENT"      */
    char * pdu;     /* PDU of message to write, as an ASCII hex string less the SMSC address,
                       the TP-layer length is "strlen(pdu)/2". */
    char * smsc;    /* SMSC address in GSM BCD format prefixed by a length byte
                       (as expected by TS 27.005) or NULL for default SMSC */
    int index;
} RIL_SMS_WriteArgs;

typedef struct {
	char pdu[180];
}RIL_SMS_from_SIM;				
				
typedef struct { 				
	int sim_status[2];
}RIL_Get_Stored_Msg_Count;

typedef struct { 				
	int 	size;		  // @field structure size in bytes
	int     dataLen;	  // @field data size in bytes		
	int 	params;    // @field indicates valid parameters		
	int 	status; 	// @field additional status for message	
	char    *data;	 // @field message itself		
}RIL_SS_Release_Comp_Msg;		

//add by jiyoon

#define CellBroadcast true


#ifdef CellBroadcast

#define GSM_SMS_TPDU_STR_MAX_SIZE 490

typedef struct {

	int	bCBEnabled;/*< CB service state. If cb_enabled is true then cell broadcast service will be enabled */
	                              /* and underlying modem will enable CB Channel to receiving CB messages. Otherwise CB */    
	                              /* service will be disabled; underlying modem will deactivate the CB channel. */
	                              /* (enabled/disabled)   : Storage : MODEM Flash */ 
								  
	int	selectedId;/* < CBMI Identifier selected (all or some)  Storage : MODEM Flash */

	int	msgIdMaxCount; /*< CB Channel List Max Count , this number is different for 2G & 3G & is SIM specific Storage : MODEM Flash  */

	int	msgIdCount; /**< CB message ID count , My Channels list is updated in  Storage : MODEM Flash */

	char * msgIDs; /* < CB message ID information, */
				   /* My Channels List is updated here. Storage : SIM Memory  */
} RIL_CB_ConfigArgs ;

typedef struct {
	
	int cbType; // 0x01 :GSM , 0x02:UMTS
	int message_length;
	char message[GSM_SMS_TPDU_STR_MAX_SIZE]; //reference : 3GPP TS 23.041 9.4.1, 9.4.2
	
}RIL_CBMsg_Notification;

#endif

#define LINE_SMS_ADDRESS_SIZE 20
#define LINE_SMS_DATA_SIZE 256

typedef struct {

	int addrLen; // length of originator address
	char * address; //originator address (size is 20 bytes)
	int dataLen; //data length
	char * data;// content of SMS(8 bit ascii encoding) (size is 256 bytes)
	
	
}RIL_LineSmsReadMsg;


/** Used by RIL_REQUEST_DIAL */
typedef struct {
    char * address;
    int clir;
            /* (same as 'n' paremeter in TS 27.007 7.7 "+CLIR"
             * clir == 0 on "use subscription default value"
             * clir == 1 on "CLIR invocation" (restrict CLI presentation)
             * clir == 2 on "CLIR suppression" (allow CLI presentation)
             */
    RIL_UUS_Info *  uusInfo;    /* NULL or Pointer to User-User Signaling Information */
} RIL_Dial;

//[CONFIG_VT_SUPPORT
/** Used by RIL_REQUEST_DIAL_VIDEO_CALL */
typedef struct {
    char * address;
    int clir;           

} RIL_Video_Dial;
//]

typedef struct {
    int command;    /* one of the commands listed for TS 27.007 +CRSM*/
    int fileid;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
} RIL_SIM_IO;

typedef struct {
    int sw1;
    int sw2;
    char *simResponse;  /* In hex string format ([a-fA-F0-9]*). */
} RIL_SIM_IO_Response;

typedef struct {
    int command;    /*add, update, delete*/
    int fileid;     /* EF id */
	int index;		/* record index */
	char *alphaTag;     /* name     */
	int alphaTagDCS;
	int alphaTagLength;
	char *number;     /* number     */	
	char *email;
	int emailTagDCS;	//HJKIM_emailDCS
	int emailLength;
    char *pin2;     /* May be NULL*/
} RIL_SIM_ACCESS_PB;


typedef struct {
    int command;    /* add,get,update*/
    int fileid;     /* EF id */
	int index;   
    char *pin2;     /* May be NULL*/
} RIL_SIM_PB;

/*index for text RIL_SIM_PB_Response*/
typedef enum {
	INDEX_NAME = 0,
	INDEX_SNE,
	INDEX_EMAIL,
	NUM_OF_ALPHA,
} INDEX_TEXT_RIL_SIM_PB_Response;
/*index for num RIL_SIM_PB_Response*/
typedef enum {
	INDEX_NUMBER = 0,
	INDEX_ANR,
	INDEX_ANRA,
	INDEX_ANRB,
	NUM_OF_NUMBER,
} INDEX_NUM_RIL_SIM_PB_Response;

typedef struct {
	int lengthAlphas[NUM_OF_ALPHA];
	int dataTypeAlphas[NUM_OF_ALPHA];
	char* alphaTags[NUM_OF_ALPHA];

	int lengthNumbers[NUM_OF_NUMBER];
	int dataTypeNumbers[NUM_OF_NUMBER];
	char* numbers[NUM_OF_NUMBER];

	int recordIndex;
	int nextIndex;
} RIL_SIM_PB_Response;


/* See also com.android.internal.telephony.gsm.CallForwardInfo */

typedef struct {
    int             status;     /*
                                 * For RIL_REQUEST_QUERY_CALL_FORWARD_STATUS
                                 * status 1 = active, 0 = not active
                                 *
                                 * For RIL_REQUEST_SET_CALL_FORWARD:
                                 * status is:
                                 * 0 = disable
                                 * 1 = enable
                                 * 2 = interrogate
                                 * 3 = registeration
                                 * 4 = erasure
                                 */

    int             reason;      /* from TS 27.007 7.11 "reason" */
    int             serviceClass;/* From 27.007 +CCFC/+CLCK "class"
                                    See table for Android mapping from
                                    MMI service code
                                    0 means user doesn't input class */
    int             toa;         /* "type" from TS 27.007 7.11 */
    char *          number;      /* "number" from TS 27.007 7.11. May be NULL */
    int             timeSeconds; /* for CF no reply only */
}RIL_CallForwardInfo;

typedef struct {
   char * cid;         /* Combination of LAC and Cell Id in 32 bits in GSM.
                        * Upper 16 bits is LAC and lower 16 bits
                        * is CID (as described in TS 27.005)
                        * Primary Scrambling Code (as described in TS 25.331)
                        *         in 9 bits in UMTS
                        * Valid values are hexadecimal 0x0000 - 0xffffffff.
                        */
   int    rssi;        /* Received RSSI in GSM,
                        * Level index of CPICH Received Signal Code Power in UMTS
                        */
} RIL_NeighboringCell;

/* See RIL_REQUEST_LAST_CALL_FAIL_CAUSE */
typedef enum {
    CALL_FAIL_UNOBTAINABLE_NUMBER = 1,
    CALL_FAIL_NORMAL = 16,
    CALL_FAIL_BUSY = 17,
    CALL_FAIL_CONGESTION = 34,
    CALL_FAIL_ACM_LIMIT_EXCEEDED = 68,
    CALL_FAIL_CALL_BARRED = 240,
    CALL_FAIL_FDN_BLOCKED = 241,
    CALL_FAIL_IMSI_UNKNOWN_IN_VLR = 242,
    CALL_FAIL_IMEI_NOT_ACCEPTED = 243,
    CALL_FAIL_CDMA_LOCKED_UNTIL_POWER_CYCLE = 1000,
    CALL_FAIL_CDMA_DROP = 1001,
    CALL_FAIL_CDMA_INTERCEPT = 1002,
    CALL_FAIL_CDMA_REORDER = 1003,
    CALL_FAIL_CDMA_SO_REJECT = 1004,
    CALL_FAIL_CDMA_RETRY_ORDER = 1005,
    CALL_FAIL_CDMA_ACCESS_FAILURE = 1006,
    CALL_FAIL_CDMA_PREEMPTED = 1007,
    CALL_FAIL_CDMA_NOT_EMERGENCY = 1008, /* For non-emergency number dialed
                                            during emergency callback mode */
    CALL_FAIL_CDMA_ACCESS_BLOCKED = 1009, /* CDMA network access probes blocked */
    CALL_FAIL_ERROR_UNSPECIFIED = 0xffff
} RIL_LastCallFailCause;

/* See RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE */
typedef enum {
    PDP_FAIL_OPERATOR_BARRED = 0x08,               /* no retry */
    PDP_FAIL_INSUFFICIENT_RESOURCES = 0x1A,
    PDP_FAIL_MISSING_UKNOWN_APN = 0x1B,            /* no retry */
    PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE = 0x1C,      /* no retry */
    PDP_FAIL_USER_AUTHENTICATION = 0x1D,           /* no retry */
    PDP_FAIL_ACTIVATION_REJECT_GGSN = 0x1E,        /* no retry */
    PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED = 0x1F,
    PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED = 0x20,  /* no retry */
    PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED = 0x21, /* no retry */
    PDP_FAIL_SERVICE_OPTION_OUT_OF_ORDER = 0x22,
    PDP_FAIL_NSAPI_IN_USE      = 0x23,             /* no retry */
    PDP_FAIL_PROTOCOL_ERRORS   = 0x6F,             /* no retry */
    PDP_FAIL_ERROR_UNSPECIFIED = 0xffff,  /* This and all other cases: retry silently */
    /* Not mentioned in the specification */
    PDP_FAIL_REGISTRATION_FAIL = -1,
    PDP_FAIL_GPRS_REGISTRATION_FAIL = -2,
} RIL_LastDataCallActivateFailCause;

/* See RIL_REQUEST_SETUP_DATA_CALL */
typedef enum {
    RIL_DATA_PROFILE_DEFAULT    = 0,
    RIL_DATA_PROFILE_TETHERED   = 1,
    RIL_DATA_PROFILE_OEM_BASE   = 1000    /* Start of OEM-specific profiles */
} RIL_DataProfile;

/* Used by RIL_UNSOL_SUPP_SVC_NOTIFICATION */
typedef struct {
    int     notificationType;   /*
                                 * 0 = MO intermediate result code
                                 * 1 = MT unsolicited result code
                                 */
    int     code;               /* See 27.007 7.17
                                   "code1" for MO
                                   "code2" for MT. */
    int     index;              /* CUG index. See 27.007 7.17. */
    int     type;               /* "type" from 27.007 7.17 (MT only). */
    char *  number;             /* "number" from 27.007 7.17
                                   (MT only, may be NULL). */
} RIL_SuppSvcNotification;


/* Used by RIL_UNSOL_RELEASE_COMPLETE_MESSAGE */
typedef struct {
    int size;         // @field structure size in bytes
    int dataLen;    // @field data size in bytes
    int params;	   // @field indicates valid parameters
    int status;     // @field additional status for message
    char* data;     // @field message itself
} RIL_SSReleaseComplete;

typedef enum{				
	SRIL_MN_SMS_NO_ERROR = 0x0000,/* MN_SMS_NO_ERROR in msnu.h*/			
	SRIL_MN_SMS_GENERIC_ERROR = 0x0001,/* all the error except above three in msnu.h*/			
	SRIL_MN_SMS_MEMORY_CAPACITY_EXCEEDED = 0x8016, /* MN_SMS_MEMORY_CAPACITY_EXCEEDED =  3 T_SMS_PP_CAUSE in msnu.h*/			
	SRIL_MN_SMS_NETWORK_OUT_OF_ORDER = 0x802A,/* MN_SMS_NETWORK_OUT_OF_ORDER in msnu.h , when retry has failed*/			
}RIL_SMS_Result;				
				
	
typedef struct { 				
    char call_type;         				
    char control_result;    			
    char alpha_id_present;				
    char alpha_id_len;				
    char alpha_id[64];				
    char call_id;				
    char old_call_type;   				
    char modadd_ton;				
    char modadd_npi;				
    char modadd_len;				
    char modadd[200];				
}RIL_Stk_CallCtrl_Result;				

#define RIL_PARAM_SSDI_STATUS	1
#define RIL_PARAM_SSDI_DATA 		2


typedef struct {
    char* encodedUSSD;
    int ussdLength;
    int dcsCode;
} RIL_EncodedUSSD;

#define RIL_CARD_MAX_APPS     8

typedef enum {
    RIL_CARDSTATE_ABSENT   = 0,
    RIL_CARDSTATE_PRESENT  = 1,
    RIL_CARDSTATE_ERROR    = 2
} RIL_CardState;

typedef enum {
    RIL_PERSOSUBSTATE_UNKNOWN                   = 0, /* initial state */
    RIL_PERSOSUBSTATE_IN_PROGRESS               = 1, /* in between each lock transition */
    RIL_PERSOSUBSTATE_READY                     = 2, /* when either SIM or RUIM Perso is finished
                                                        since each app can only have 1 active perso
                                                        involved */
    RIL_PERSOSUBSTATE_SIM_NETWORK               = 3,
    RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET        = 4,
    RIL_PERSOSUBSTATE_SIM_CORPORATE             = 5,
    RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER      = 6,
    RIL_PERSOSUBSTATE_SIM_SIM                   = 7,
    RIL_PERSOSUBSTATE_SIM_NETWORK_PUK           = 8, /* The corresponding perso lock is blocked */
    RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET_PUK    = 9,
    RIL_PERSOSUBSTATE_SIM_CORPORATE_PUK         = 10,
    RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER_PUK  = 11,
    RIL_PERSOSUBSTATE_SIM_SIM_PUK               = 12,
    RIL_PERSOSUBSTATE_RUIM_NETWORK1             = 13,
    RIL_PERSOSUBSTATE_RUIM_NETWORK2             = 14,
    RIL_PERSOSUBSTATE_RUIM_HRPD                 = 15,
    RIL_PERSOSUBSTATE_RUIM_CORPORATE            = 16,
    RIL_PERSOSUBSTATE_RUIM_SERVICE_PROVIDER     = 17,
    RIL_PERSOSUBSTATE_RUIM_RUIM                 = 18,
    RIL_PERSOSUBSTATE_RUIM_NETWORK1_PUK         = 19, /* The corresponding perso lock is blocked */
    RIL_PERSOSUBSTATE_RUIM_NETWORK2_PUK         = 20,
    RIL_PERSOSUBSTATE_RUIM_HRPD_PUK             = 21,
    RIL_PERSOSUBSTATE_RUIM_CORPORATE_PUK        = 22,
    RIL_PERSOSUBSTATE_RUIM_SERVICE_PROVIDER_PUK = 23,
    RIL_PERSOSUBSTATE_RUIM_RUIM_PUK             = 24
} RIL_PersoSubstate;

typedef enum {
    RIL_APPSTATE_UNKNOWN               = 0,
    RIL_APPSTATE_DETECTED              = 1,
    RIL_APPSTATE_PIN                   = 2, /* If PIN1 or UPin is required */
    RIL_APPSTATE_PUK                   = 3, /* If PUK1 or Puk for UPin is required */
    RIL_APPSTATE_SUBSCRIPTION_PERSO    = 4, /* perso_substate should be look at
                                               when app_state is assigned to this value */
    RIL_APPSTATE_READY                 = 5,
    RIL_APPSTATE_ILLEGAL               = 6 /* App State marked as Illegal by L3 protocol layer */
} RIL_AppState;

typedef enum {
    RIL_PINSTATE_UNKNOWN              = 0,
    RIL_PINSTATE_ENABLED_NOT_VERIFIED = 1,
    RIL_PINSTATE_ENABLED_VERIFIED     = 2,
    RIL_PINSTATE_DISABLED             = 3,
    RIL_PINSTATE_ENABLED_BLOCKED      = 4,
    RIL_PINSTATE_ENABLED_PERM_BLOCKED = 5
} RIL_PinState;

typedef enum {
  RIL_APPTYPE_UNKNOWN = 0,
  RIL_APPTYPE_SIM     = 1,
  RIL_APPTYPE_USIM    = 2,
  RIL_APPTYPE_RUIM    = 3,
  RIL_APPTYPE_CSIM    = 4
} RIL_AppType;

typedef struct
{
  RIL_AppType      app_type;
  RIL_AppState     app_state;
  RIL_PersoSubstate perso_substate; /* applicable only if app_state ==
                                       RIL_APPSTATE_SUBSCRIPTION_PERSO */
  char             *aid_ptr;        /* null terminated string, e.g., from 0xA0, 0x00 -> 0x41,
                                       0x30, 0x30, 0x30 */
  char             *app_label_ptr;  /* null terminated string */
  int              pin1_replaced;   /* applicable to USIM and CSIM */
  RIL_PinState     pin1;
  RIL_PinState     pin2;
} RIL_AppStatus;

typedef struct
{
  RIL_CardState card_state;
  RIL_PinState  universal_pin_state;             /* applicable to USIM and CSIM: RIL_PINSTATE_xxx */
  int           gsm_umts_subscription_app_index; /* value < RIL_CARD_MAX_APPS */
  int           cdma_subscription_app_index;     /* value < RIL_CARD_MAX_APPS */
  int           num_applications;                /* value <= RIL_CARD_MAX_APPS */
  RIL_AppStatus applications[RIL_CARD_MAX_APPS];
} RIL_CardStatus;

/* The result of a SIM refresh, returned in data[0] of RIL_UNSOL_SIM_REFRESH */
typedef enum {
    /* A file on SIM has been updated.  data[1] contains the EFID. */
    SIM_FILE_UPDATE = 0,
    /* SIM initialized.  All files should be re-read. */
    SIM_INIT = 1,
    /* SIM reset.  SIM power required, SIM may be locked and all files should be re-read. */
    SIM_RESET = 2
} RIL_SimRefreshResult;

typedef struct {
    char *          number;             /* Remote party number */
    int             numberPresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown */
    char *          name;               /* Remote party name */
    RIL_CDMA_SignalInfoRecord signalInfoRecord;
} RIL_CDMA_CallWaiting;

/**
 * Which types of Cell Broadcast Message (CBM) are to be received by the ME
 *
 * uFromServiceID - uToServiceID defines a range of CBM message identifiers
 * whose value is 0x0000 - 0xFFFF as defined in TS 23.041 9.4.1.2.2 for GMS
 * and 9.4.4.2.2 for UMTS. All other values can be treated as empty
 * CBM message ID.
 *
 * uFromCodeScheme - uToCodeScheme defines a range of CBM data coding schemes
 * whose value is 0x00 - 0xFF as defined in TS 23.041 9.4.1.2.3 for GMS
 * and 9.4.4.2.3 for UMTS.
 * All other values can be treated as empty CBM data coding scheme.
 *
 * selected 0 means message types specified in <fromServiceId, toServiceId>
 * and <fromCodeScheme, toCodeScheme>are not accepted, while 1 means accepted.
 *
 * Used by RIL_REQUEST_GSM_GET_BROADCAST_CONFIG and
 * RIL_REQUEST_GSM_SET_BROADCAST_CONFIG.
 */
typedef struct {
    int fromServiceId;
    int toServiceId;
    int fromCodeScheme;
    int toCodeScheme;
    unsigned char selected;
} RIL_GSM_BroadcastSmsConfigInfo;

/* No restriction at all including voice/SMS/USSD/SS/AV64 and packet data. */
#define RIL_RESTRICTED_STATE_NONE           0x00
/* Block emergency call due to restriction. But allow all normal voice/SMS/USSD/SS/AV64. */
#define RIL_RESTRICTED_STATE_CS_EMERGENCY   0x01
/* Block all normal voice/SMS/USSD/SS/AV64 due to restriction. Only Emergency call allowed. */
#define RIL_RESTRICTED_STATE_CS_NORMAL      0x02
/* Block all voice/SMS/USSD/SS/AV64 including emergency call due to restriction.*/
#define RIL_RESTRICTED_STATE_CS_ALL         0x04
/* Block packet data access due to restriction. */
#define RIL_RESTRICTED_STATE_PS_ALL         0x10

/* The status for an OTASP/OTAPA session */
typedef enum {
    CDMA_OTA_PROVISION_STATUS_SPL_UNLOCKED,
    CDMA_OTA_PROVISION_STATUS_SPC_RETRIES_EXCEEDED,
    CDMA_OTA_PROVISION_STATUS_A_KEY_EXCHANGED,
    CDMA_OTA_PROVISION_STATUS_SSD_UPDATED,
    CDMA_OTA_PROVISION_STATUS_NAM_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_MDN_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_IMSI_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_PRL_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_COMMITTED,
    CDMA_OTA_PROVISION_STATUS_OTAPA_STARTED,
    CDMA_OTA_PROVISION_STATUS_OTAPA_STOPPED,
    CDMA_OTA_PROVISION_STATUS_OTAPA_ABORTED
} RIL_CDMA_OTA_ProvisionStatus;

typedef struct {
    int signalStrength;  /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int bitErrorRate;    /* bit error rate (0-7, 99) as defined in TS 27.007 8.5 */
} RIL_GW_SignalStrength;


typedef struct {
    int dbm;  /* Valid values are positive integers.  This value is the actual RSSI value
               * multiplied by -1.  Example: If the actual RSSI is -75, then this response
               * value will be 75.
               */
    int ecio; /* Valid values are positive integers.  This value is the actual Ec/Io multiplied
               * by -10.  Example: If the actual Ec/Io is -12.5 dB, then this response value
               * will be 125.
               */
} RIL_CDMA_SignalStrength;


typedef struct {
    int dbm;  /* Valid values are positive integers.  This value is the actual RSSI value
               * multiplied by -1.  Example: If the actual RSSI is -75, then this response
               * value will be 75.
               */
    int ecio; /* Valid values are positive integers.  This value is the actual Ec/Io multiplied
               * by -10.  Example: If the actual Ec/Io is -12.5 dB, then this response value
               * will be 125.
               */
    int signalNoiseRatio; /* Valid values are 0-8.  8 is the highest signal to noise ratio. */
} RIL_EVDO_SignalStrength;


typedef struct {
    RIL_GW_SignalStrength   GW_SignalStrength;
    RIL_CDMA_SignalStrength CDMA_SignalStrength;
    RIL_EVDO_SignalStrength EVDO_SignalStrength;
} RIL_SignalStrength;

/* Names of the CDMA info records (C.S0005 section 3.7.5) */
typedef enum {
  RIL_CDMA_DISPLAY_INFO_REC,
  RIL_CDMA_CALLED_PARTY_NUMBER_INFO_REC,
  RIL_CDMA_CALLING_PARTY_NUMBER_INFO_REC,
  RIL_CDMA_CONNECTED_NUMBER_INFO_REC,
  RIL_CDMA_SIGNAL_INFO_REC,
  RIL_CDMA_REDIRECTING_NUMBER_INFO_REC,
  RIL_CDMA_LINE_CONTROL_INFO_REC,
  RIL_CDMA_EXTENDED_DISPLAY_INFO_REC,
  RIL_CDMA_T53_CLIR_INFO_REC,
  RIL_CDMA_T53_RELEASE_INFO_REC,
  RIL_CDMA_T53_AUDIO_CONTROL_INFO_REC
} RIL_CDMA_InfoRecName;

/* Display Info Rec as defined in C.S0005 section 3.7.5.1
   Extended Display Info Rec as defined in C.S0005 section 3.7.5.16
   Note: the Extended Display info rec contains multiple records of the
   form: display_tag, display_len, and display_len occurrences of the
   chari field if the display_tag is not 10000000 or 10000001.
   To save space, the records are stored consecutively in a byte buffer.
   The display_tag, display_len and chari fields are all 1 byte.
*/

typedef struct {
  char alpha_len;
  char alpha_buf[CDMA_ALPHA_INFO_BUFFER_LENGTH];
} RIL_CDMA_DisplayInfoRecord;

/* Called Party Number Info Rec as defined in C.S0005 section 3.7.5.2
   Calling Party Number Info Rec as defined in C.S0005 section 3.7.5.3
   Connected Number Info Rec as defined in C.S0005 section 3.7.5.4
*/

typedef struct {
  char len;
  char buf[CDMA_NUMBER_INFO_BUFFER_LENGTH];
  char number_type;
  char number_plan;
  char pi;
  char si;
} RIL_CDMA_NumberInfoRecord;

/* Redirecting Number Information Record as defined in C.S0005 section 3.7.5.11 */
typedef enum {
  RIL_REDIRECTING_REASON_UNKNOWN = 0,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_BUSY = 1,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_NO_REPLY = 2,
  RIL_REDIRECTING_REASON_CALLED_DTE_OUT_OF_ORDER = 9,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_BY_THE_CALLED_DTE = 10,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_UNCONDITIONAL = 15,
  RIL_REDIRECTING_REASON_RESERVED
} RIL_CDMA_RedirectingReason;

typedef struct {
  RIL_CDMA_NumberInfoRecord redirectingNumber;
  /* redirectingReason is set to RIL_REDIRECTING_REASON_UNKNOWN if not included */
  RIL_CDMA_RedirectingReason redirectingReason;
} RIL_CDMA_RedirectingNumberInfoRecord;

/* Line Control Information Record as defined in C.S0005 section 3.7.5.15 */
typedef struct {
  char lineCtrlPolarityIncluded;
  char lineCtrlToggle;
  char lineCtrlReverse;
  char lineCtrlPowerDenial;
} RIL_CDMA_LineControlInfoRecord;

/* T53 CLIR Information Record */
typedef struct {
  char cause;
} RIL_CDMA_T53_CLIRInfoRecord;

/* T53 Audio Control Information Record */
typedef struct {
  char upLink;
  char downLink;
} RIL_CDMA_T53_AudioControlInfoRecord;

typedef struct {

  RIL_CDMA_InfoRecName name;

  union {
    /* Display and Extended Display Info Rec */
    RIL_CDMA_DisplayInfoRecord           display;

    /* Called Party Number, Calling Party Number, Connected Number Info Rec */
    RIL_CDMA_NumberInfoRecord            number;

    /* Signal Info Rec */
    RIL_CDMA_SignalInfoRecord            signal;

    /* Redirecting Number Info Rec */
    RIL_CDMA_RedirectingNumberInfoRecord redir;

    /* Line Control Info Rec */
    RIL_CDMA_LineControlInfoRecord       lineCtrl;

    /* T53 CLIR Info Rec */
    RIL_CDMA_T53_CLIRInfoRecord          clir;

    /* T53 Audio Control Info Rec */
    RIL_CDMA_T53_AudioControlInfoRecord  audioCtrl;
  } rec;
} RIL_CDMA_InformationRecord;

#define RIL_CDMA_MAX_NUMBER_OF_INFO_RECS 10

typedef struct {
  char numberOfInfoRecs;
  RIL_CDMA_InformationRecord infoRec[RIL_CDMA_MAX_NUMBER_OF_INFO_RECS];
} RIL_CDMA_InformationRecords;

typedef enum
{
	SRIL_LOCK_READY = 0x00,
	SRIL_LOCK_PH_SIM = 0x01, // SIM LOCK (Personalisation)
	SRIL_LOCK_PH_FSIM = 0x02, //SIM LOCK (Personalisation)
	SRIL_LOCK_SIM = 0x03, // PIN LOCK
	SRIL_LOCK_FD = 0x04, // Fixed Dialing Memeory feature
	SRIL_LOCK_NETWORK_PERS = 0x05,
	SRIL_LOCK_NETWORK_SUBSET_PERS = 0x06,
	SRIL_LOCK_SP_PERS = 0x07,
	SRIL_LOCK_CORP_PERS = 0x08,
	SRIL_LOCK_PIN2 = 0x09,
	SRIL_LOCK_PUK2 = 0x0A,    
	SRIL_LOCK_ACL = 0x0B,    
	SRIL_LOCK_NO_SIM = 0x80 
}SRIL_Lock_Type_t;				

typedef enum
{
	SRIL_PIN_NOT_NEED = 0x00,
	SRIL_PIN = 0x01,
	SRIL_PUK = 0x02,
	SRIL_PIN2 = 0x03,
	SRIL_PUK2 = 0x04,
	SRIL_PERM_BLOCKED = 0x05,
	SRIL_PIN2_DISABLE = 0x06
}SRIL_LOCK_STATUS;

typedef struct {
	int num_lock_type;
	int lock_type;
} RIL_SIM_Lockinfo;

typedef struct {
	int num_lock_type;
	int lock_type;
	int lock_key;
	int num_of_retry;
} RIL_SIM_Lockinfo_Response;

typedef struct { 				
 /* data is int[1] that has following elements:				
 				
   data[0] - A phonebook storage type selected.				
             It has following values:				
 				
             PB_DC    0x01 ME dialed calls list stored in NV				
             PB_EN    0x02 SIM(or ME) emergency number				
             PB_FD    0x03 SIM fixed-dialing phonebook				
             PB_LD    0x04 SIM last-dialing phonebook				
             PB_MC    0x05 ME missed calls list				
             PB_ME    0x06 ME phonebook				
             PB_MT    0x07 Combined ME and SIM phonebook				
             PB_ON    0x08 SIM(or ME) own numbers ( MSISDNs) list				
             PB_RC    0x09 ME received calls list stored in NV				
             PB_SIM   0x0A 2G SIM phonebook				
             PB_SDN   0x0B Service Dialing Number				
             PB_3GSIM 0x0C 3G SIM phonebook				
             PB_ICI   0x0D Incoming Call Information				
             PB_OCI   0x0E Outgoing Call Information				
             PB_AAS   0x0F Additional Number Alpha String				
             PB_GAS   0x10 Grouping Information Alpha String				
*/				
				
 int data;				
				
}SRIL_Phonebk_Storage_Info_request;				
				
typedef struct { 				
/*				
   response[0] - Total slot count.				
                 The number of the total phonebook memory.				
                 according to Phone book storage.				
   response[1] - Used slot count.				
                 The number of the used phonebook memory.				
   response[2] - First index.				
                 Minimum index of phonebook entries.				
   response[3] - Maximum number of phonebook entry text field.				
   resopnse[4] - Maximum number of phonebook entry number field.				
*/				
				
  int total;				
  int used;				
  int first_id;				
  int max_text;				
  int max_num;				
  				
}RIL_Phonebk_Storage_Info;	

#define MAX_3GPP_TYPE 13
#define MAX_DATA_LEN 4
typedef struct {

 
	int response[MAX_3GPP_TYPE][MAX_DATA_LEN];
	
/*
	"response" is int[MAX_3GPP_TYPE(13) MAX_DATA_LEN(4)] type.
 
   response[0] PB_FIELD_TYPE
   response[1] MAX_INDEX     Maximum index of phonebook field entries
   response[2] MAX_ENTRY     Maximum number of phonebook field entry field
   response[3] USED RECORD   Record in use of phoebook field (except 3GPP_PBC )


 
   PB_FIELD_TYPE has following values:
 
   FIELD_3GPP_NAME   = 0x01 : Name of current storageg
   FIELD_3GPP_NUMBER = 0x02 : Number of current storageg
   FIELD_3GPP_ANR    = 0x03 : First additional numberg
   FIELD_3GPP_EMAIL  = 0x04 : First email addressg
   FIELD_3GPP_SNE    = 0x05 : Second name entryg
   FIELD_3GPP_GRP    = 0x06 : Grouping fileg
   FIELD_3GPP_PBC    = 0x07 : Phonebook controlg
   FIELD_3GPP_ANRA   = 0x08 : Second additional numberg
   FIELD_3GPP_ANRB   = 0x09 : Third additional numberg
   FIELD_3GPP_ANRC   = 0x0a : Fourth additional numberg
   FIELD_3GPP_EMAILA = 0x0b : Second email addressg
   FIELD_3GPP_EMAILB = 0x0c : Third email addressg
   FIELD_3GPP_EMAILC = 0x0d : Fourth email addressg
 */
}RIL_Usim_PB_Capa;

#define NUM_OF_ALPHA 3				
#define NUM_OF_NUMBER 4				
				
/* RIL_REQUEST_GET_PHONEBOOK_ENTRY response*/				
typedef struct { 				
				
   int lengthAlphas[NUM_OF_ALPHA];	/* length of each alphaTags[i] */		
				
   int dataTypeAlphas[NUM_OF_ALPHA];	/*			
				
     character set of each of <alphaTags[i]>.				
				
     Now, following values are available:				
				
       GSM 7BIT = 0x02: <alphaTag> is converted a string into				
                        an 8-bit unpacked GSM alphabet byte array.				
				
       UCS2     = 0x03: <alphaTag> is converted a string into				
                        an 16-bit universal multiple-octet coded				
                        character set (ISO/IEC10646)				
				
                        it will be read by UTF-16.				
				
       others are not supported.				
				
     dataTypeAlphas[NAME  = 0];	/* character set of name that corresponds to				
                                <text> field of +CPBR.				
				
     dataTypeAlphas[SNE   = 1]: character set of secondary name entry that corresponds to				
                                <secondtext> field of +CPBR.				
				
                                it is not used parameter now, because SNE field is not used.				
				
     dataTypeAlphas[EMAIL = 2]: character set of first email address that corresopnds to				
                                <email> field of +CPBR.	
                                */
 				
   char* alphaTags[NUM_OF_ALPHA ];	/*hex string to specify following fields 				
				
     alphaTags[NAME  = 0]: name that corresponds to <text> field of +CPBR.				
     				
     alphaTags[SNE   = 1]: second name entry that corresponnds to				
                           <secondtext> field of +CPBR.				
				
                           it is not used parameter now.				
				
     alphaTags[EMAIL = 2]: first email address that corresponds to				
                           <email> field of +CPBR.	*/			
				
   				
   int lengthNumbers[NUM_OF_NUMBER ];	/* length of each numbers[i].	*/			
				
   int dataTypeNumbers[NUM_OF_NUMBER];	/* data type or format of each numbers[i].				
 				
                                           it is not used parameter now.	*/			
				
   char* numbers[NUM_OF_NUMBER ];	/*				
				
     numbers[INDEX_NUMBER = 0] number that corresponds to <number> field of +CPBR.				
				
     numbers[INDEX_ANR    = 1] second additional number that corresponds to				
                                <adnumber> field of +CPBR.				
				
                                it is not used parameter now.				
				
     numbers[INDEX_ANRA   = 2]: third additional number.				
				
                                it is not used parameter now.				
				
     numbers[INDEX_ANRB   = 3]: fourth additional number.				
 				
                                it is not used parameter now. */				
				
   int recordIndex ;	/* location which the numbers are stored				
                                          in the storage.				
                                          it can be corresponded to <index> field of +CPBR.*/				
				
   int nextIndex ;	/* location of the next storage from current number.				
                                          it can be corresponded to next <index> field				
                                          of +CPBR.		*/				
				
   				
}RIL_Phonebk_Entry_rp;	

/**
 * RIL_REQUEST_GET_SIM_STATUS
 *
 * Requests status of the SIM interface and the SIM card
 *
 * "data" is NULL
 *
 * "response" is const RIL_CardStatus *
 *
 * Valid errors:
 *  Must never fail
 */
#define RIL_REQUEST_GET_SIM_STATUS 1

/**
 * RIL_REQUEST_ENTER_SIM_PIN
 *
 * Supplies SIM PIN. Only called if RIL_CardStatus has RIL_APPSTATE_PIN state
 *
 * "data" is const char **
 * ((const char **)data)[0] is PIN value
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 * SUCCESS
 * RADIO_NOT_AVAILABLE (radio resetting)
 * GENERIC_FAILURE
 * PASSWORD_INCORRECT
 */

#define RIL_REQUEST_ENTER_SIM_PIN 2


/**
 * RIL_REQUEST_ENTER_SIM_PUK
 *
 * Supplies SIM PUK and new PIN.
 *
 * "data" is const char **
 * ((const char **)data)[0] is PUK value
 * ((const char **)data)[1] is new PIN value
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (PUK is invalid)
 */

#define RIL_REQUEST_ENTER_SIM_PUK 3

/**
 * RIL_REQUEST_ENTER_SIM_PIN2
 *
 * Supplies SIM PIN2. Only called following operation where SIM_PIN2 was
 * returned as a a failure from a previous operation.
 *
 * "data" is const char **
 * ((const char **)data)[0] is PIN2 value
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 */

#define RIL_REQUEST_ENTER_SIM_PIN2 4

/**
 * RIL_REQUEST_ENTER_SIM_PUK2
 *
 * Supplies SIM PUK2 and new PIN2.
 *
 * "data" is const char **
 * ((const char **)data)[0] is PUK2 value
 * ((const char **)data)[1] is new PIN2 value
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (PUK2 is invalid)
 */

#define RIL_REQUEST_ENTER_SIM_PUK2 5

/**
 * RIL_REQUEST_CHANGE_SIM_PIN
 *
 * Supplies old SIM PIN and new PIN.
 *
 * "data" is const char **
 * ((const char **)data)[0] is old PIN value
 * ((const char **)data)[1] is new PIN value
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (old PIN is invalid)
 *
 */

#define RIL_REQUEST_CHANGE_SIM_PIN 6


/**
 * RIL_REQUEST_CHANGE_SIM_PIN2
 *
 * Supplies old SIM PIN2 and new PIN2.
 *
 * "data" is const char **
 * ((const char **)data)[0] is old PIN2 value
 * ((const char **)data)[1] is new PIN2 value
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (old PIN2 is invalid)
 *
 */

#define RIL_REQUEST_CHANGE_SIM_PIN2 7

/**
 * RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION
 *
 * Requests that network personlization be deactivated
 *
 * "data" is const char **
 * ((const char **)(data))[0]] is network depersonlization code
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (code is invalid)
 */

#define RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION 8

/**
 * RIL_REQUEST_GET_CURRENT_CALLS
 *
 * Requests current call list
 *
 * "data" is NULL
 *
 * "response" must be a "const RIL_Call **"
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *      (request will be made again in a few hundred msec)
 */

#define RIL_REQUEST_GET_CURRENT_CALLS 9


/**
 * RIL_REQUEST_DIAL
 *
 * Initiate voice call
 *
 * "data" is const RIL_Dial *
 * "response" is NULL
 *
 * This method is never used for supplementary service codes
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DIAL 10

/**
 * RIL_REQUEST_GET_IMSI
 *
 * Get the SIM IMSI
 *
 * Only valid when radio state is "RADIO_STATE_SIM_READY"
 *
 * "data" is NULL
 * "response" is a const char * containing the IMSI
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_IMSI 11

/**
 * RIL_REQUEST_HANGUP
 *
 * Hang up a specific line (like AT+CHLD=1x)
 *
 * "data" is an int *
 * (int *)data)[0] contains Connection index (value of 'x' in CHLD above)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_HANGUP 12

/**
 * RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND
 *
 * Hang up waiting or held (like AT+CHLD=0)
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND 13

/**
 * RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND
 *
 * Hang up waiting or held (like AT+CHLD=1)
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND 14

/**
 * RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE
 *
 * Switch waiting or holding call and active call (like AT+CHLD=2)
 *
 * State transitions should be is follows:
 *
 * If call 1 is waiting and call 2 is active, then if this re
 *
 *   BEFORE                               AFTER
 * Call 1   Call 2                 Call 1       Call 2
 * ACTIVE   HOLDING                HOLDING     ACTIVE
 * ACTIVE   WAITING                HOLDING     ACTIVE
 * HOLDING  WAITING                HOLDING     ACTIVE
 * ACTIVE   IDLE                   HOLDING     IDLE
 * IDLE     IDLE                   IDLE        IDLE
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE 15
#define RIL_REQUEST_SWITCH_HOLDING_AND_ACTIVE 15

/**
 * RIL_REQUEST_CONFERENCE
 *
 * Conference holding and active (like AT+CHLD=3)

 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CONFERENCE 16

/**
 * RIL_REQUEST_UDUB
 *
 * Send UDUB (user determined used busy) to ringing or
 * waiting call answer)(RIL_BasicRequest r);
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_UDUB 17

/**
 * RIL_REQUEST_LAST_CALL_FAIL_CAUSE
 *
 * Requests the failure cause code for the most recently terminated call
 *
 * "data" is NULL
 * "response" is a "int *"
 * ((int *)response)[0] is RIL_LastCallFailCause.  GSM failure reasons are
 * mapped to cause codes defined in TS 24.008 Annex H where possible. CDMA
 * failure reasons are derived from the possible call failure scenarios
 * described in the "CDMA IS-2000 Release A (C.S0005-A v6.0)" standard.
 *
 * The implementation should return CALL_FAIL_ERROR_UNSPECIFIED for blocked
 * MO calls by restricted state (See RIL_UNSOL_RESTRICTED_STATE_CHANGED)
 *
 * If the implementation does not have access to the exact cause codes,
 * then it should return one of the values listed in RIL_LastCallFailCause,
 * as the UI layer needs to distinguish these cases for tone generation or
 * error notification.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE
 */
#define RIL_REQUEST_LAST_CALL_FAIL_CAUSE 18

/**
 * RIL_REQUEST_SIGNAL_STRENGTH
 *
 * Requests current signal strength and associated information
 *
 * Must succeed if radio is on.
 *
 * "data" is NULL
 *
 * "response" is a const RIL_SignalStrength *
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 */
#define RIL_REQUEST_SIGNAL_STRENGTH 19

/**
 * RIL_REQUEST_REGISTRATION_STATE
 *
 * Request current registration state
 *
 * "data" is NULL
 * "response" is a "char **"
 * ((const char **)response)[0] is registration state 0-6,
 *              0 - Not registered, MT is not currently searching
 *                  a new operator to register
 *              1 - Registered, home network
 *              2 - Not registered, but MT is currently searching
 *                  a new operator to register
 *              3 - Registration denied
 *              4 - Unknown
 *              5 - Registered, roaming
 *             10 - Same as 0, but indicates that emergency calls
 *                  are enabled.
 *             12 - Same as 2, but indicates that emergency calls
 *                  are enabled.
 *             13 - Same as 3, but indicates that emergency calls
 *                  are enabled.
 *             14 - Same as 4, but indicates that emergency calls
 *                  are enabled.
 *
 * ((const char **)response)[1] is LAC if registered on a GSM/WCDMA system or
 *                              NULL if not.Valid LAC are 0x0000 - 0xffff
 * ((const char **)response)[2] is CID if registered on a * GSM/WCDMA or
 *                              NULL if not.
 *                                 Valid CID are 0x00000000 - 0xffffffff
 *                                    In GSM, CID is Cell ID (see TS 27.007)
 *                                            in 16 bits
 *                                    In UMTS, CID is UMTS Cell Identity
 *                                             (see TS 25.331) in 28 bits
 * ((const char **)response)[3] indicates the available radio technology 0-7,
 *                                  0 - Unknown, 1 - GPRS, 2 - EDGE, 3 - UMTS,
 *                                  4 - IS95A, 5 - IS95B, 6 - 1xRTT,
 *                                  7 - EvDo Rev. 0, 8 - EvDo Rev. A,
 *                                  9 - HSDPA, 10 - HSUPA, 11 - HSPA,
 *                                  12 - EVDO Rev B
 * ((const char **)response)[4] is Base Station ID if registered on a CDMA
 *                              system or NULL if not.  Base Station ID in
 *                              decimal format
 * ((const char **)response)[5] is Base Station latitude if registered on a
 *                              CDMA system or NULL if not. Base Station
 *                              latitude is a decimal number as specified in
 *                              3GPP2 C.S0005-A v6.0. It is represented in
 *                              units of 0.25 seconds and ranges from -1296000
 *                              to 1296000, both values inclusive (corresponding
 *                              to a range of -90?to +90?.
 * ((const char **)response)[6] is Base Station longitude if registered on a
 *                              CDMA system or NULL if not. Base Station
 *                              longitude is a decimal number as specified in
 *                              3GPP2 C.S0005-A v6.0. It is represented in
 *                              units of 0.25 seconds and ranges from -2592000
 *                              to 2592000, both values inclusive (corresponding
 *                              to a range of -180?to +180?.
 * ((const char **)response)[7] is concurrent services support indicator if
 *                              registered on a CDMA system 0-1.
 *                                   0 - Concurrent services not supported,
 *                                   1 - Concurrent services supported
 * ((const char **)response)[8] is System ID if registered on a CDMA system or
 *                              NULL if not. Valid System ID are 0 - 32767
 * ((const char **)response)[9] is Network ID if registered on a CDMA system or
 *                              NULL if not. Valid System ID are 0 - 65535
 * ((const char **)response)[10] is the TSB-58 Roaming Indicator if registered
 *                               on a CDMA or EVDO system or NULL if not. Valid values
 *                               are 0-255.
 * ((const char **)response)[11] indicates whether the current system is in the
 *                               PRL if registered on a CDMA or EVDO system or NULL if
 *                               not. 0=not in the PRL, 1=in the PRL
 * ((const char **)response)[12] is the default Roaming Indicator from the PRL,
 *                               if registered on a CDMA or EVDO system or NULL if not.
 *                               Valid values are 0-255.
 * ((const char **)response)[13] if registration state is 3 (Registration
 *                               denied) this is an enumerated reason why
 *                               registration was denied.  See 3GPP TS 24.008,
 *                               10.5.3.6 and Annex G.
 *                                 0 - General
 *                                 1 - Authentication Failure
 *                                 2 - IMSI unknown in HLR
 *                                 3 - Illegal MS
 *                                 4 - Illegal ME
 *                                 5 - PLMN not allowed
 *                                 6 - Location area not allowed
 *                                 7 - Roaming not allowed
 *                                 8 - No Suitable Cells in this Location Area
 *                                 9 - Network failure
 *                                10 - Persistent location update reject
 *
 * Please note that registration state 4 ("unknown") is treated
 * as "out of service" in the Android telephony system
 *
 * Registration state 3 can be returned if Location Update Reject
 * (with cause 17 - Network Failure) is received repeatedly from the network,
 * to facilitate "managed roaming"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_REGISTRATION_STATE 20

/**
 * RIL_REQUEST_GPRS_REGISTRATION_STATE
 *
 * Request current GPRS registration state
 *
 * "data" is NULL
 * "response" is a "char **"
 * ((const char **)response)[0] is registration state 0-5 from TS 27.007 10.1.20 AT+CGREG
 * ((const char **)response)[1] is LAC if registered or NULL if not
 * ((const char **)response)[2] is CID if registered or NULL if not
 * ((const char **)response)[3] indicates the available radio technology, where:
 *      0 == unknown
 *      1 == GPRS only
 *      2 == EDGE
 *      3 == UMTS
 *      9 == HSDPA
 *      10 == HSUPA
 *      11 == HSPA
 *
 * LAC and CID are in hexadecimal format.
 * valid LAC are 0x0000 - 0xffff
 * valid CID are 0x00000000 - 0x0fffffff
 *
 * Please note that registration state 4 ("unknown") is treated
 * as "out of service" in the Android telephony system
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GPRS_REGISTRATION_STATE 21

/**
 * RIL_REQUEST_OPERATOR
 *
 * Request current operator ONS or EONS
 *
 * "data" is NULL
 * "response" is a "const char **"
 * ((const char **)response)[0] is long alpha ONS or EONS
 *                                  or NULL if unregistered
 *
 * ((const char **)response)[1] is short alpha ONS or EONS
 *                                  or NULL if unregistered
 * ((const char **)response)[2] is 5 or 6 digit numeric code (MCC + MNC)
 *                                  or NULL if unregistered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_OPERATOR 22

/**
 * RIL_REQUEST_RADIO_POWER
 *
 * Toggle radio on and off (for "airplane" mode)
 * "data" is int *
 * ((int *)data)[0] is > 0 for "Radio On"
 * ((int *)data)[0] is == 0 for "Radio Off"
 *
 * "response" is NULL
 *
 * Turn radio on if "on" > 0
 * Turn radio off if "on" == 0
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_RADIO_POWER 23

/**
 * RIL_REQUEST_DTMF
 *
 * Send a DTMF tone
 *
 * If the implementation is currently playing a tone requested via
 * RIL_REQUEST_DTMF_START, that tone should be cancelled and the new tone
 * should be played instead
 *
 * "data" is a char * containing a single character with one of 12 values: 0-9,*,#
 * "response" is NULL
 *
 * FIXME should this block/mute microphone?
 * How does this interact with local DTMF feedback?
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DTMF_STOP, RIL_REQUEST_DTMF_START
 *
 */
#define RIL_REQUEST_DTMF 24

/**
 * RIL_REQUEST_SEND_SMS
 *
 * Send an SMS message
 *
 * "data" is const char **
 * ((const char **)data)[0] is SMSC address in GSM BCD format prefixed
 *      by a length byte (as expected by TS 27.005) or NULL for default SMSC
 * ((const char **)data)[1] is SMS in PDU format as an ASCII hex string
 *      less the SMSC address
 *      TP-Layer-Length is be "strlen(((const char **)data)[1])/2"
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. SMS_SEND_FAIL_RETRY means retry (i.e. error cause is 332)
 * and GENERIC_FAILURE means no retry (i.e. error cause is 500)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  FDN_CHECK_FAILURE
 *  GENERIC_FAILURE
 *
 * FIXME how do we specify TP-Message-Reference if we need to resend?
 */
#define RIL_REQUEST_SEND_SMS 25


/**
 * RIL_REQUEST_SEND_SMS_EXPECT_MORE
 *
 * Send an SMS message. Identical to RIL_REQUEST_SEND_SMS,
 * except that more messages are expected to be sent soon. If possible,
 * keep SMS relay protocol link open (eg TS 27.005 AT+CMMS command)
 *
 * "data" is const char **
 * ((const char **)data)[0] is SMSC address in GSM BCD format prefixed
 *      by a length byte (as expected by TS 27.005) or NULL for default SMSC
 * ((const char **)data)[1] is SMS in PDU format as an ASCII hex string
 *      less the SMSC address
 *      TP-Layer-Length is be "strlen(((const char **)data)[1])/2"
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. SMS_SEND_FAIL_RETRY means retry (i.e. error cause is 332)
 * and GENERIC_FAILURE means no retry (i.e. error cause is 500)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SEND_SMS_EXPECT_MORE 26


/**
 * RIL_REQUEST_SETUP_DATA_CALL
 *
 * Setup a packet data connection
 *
 * "data" is a const char **
 * ((const char **)data)[0] indicates whether to setup connection on radio technology CDMA
 *                              or GSM/UMTS, 0-1. 0 - CDMA, 1-GSM/UMTS
 *
 * ((const char **)data)[1] is a RIL_DataProfile (support is optional)
 * ((const char **)data)[2] is the APN to connect to if radio technology is GSM/UMTS. This APN will
 *                          override the one in the profile. NULL indicates no APN overrride.
 * ((const char **)data)[3] is the username for APN, or NULL
 * ((const char **)data)[4] is the password for APN, or NULL
 * ((const char **)data)[5] is the PAP / CHAP auth type. Values:
 *                          0 => PAP and CHAP is never performed.
 *                          1 => PAP may be performed; CHAP is never performed.
 *                          2 => CHAP may be performed; PAP is never performed.
 *                          3 => PAP / CHAP may be performed - baseband dependent.
 *
 * "response" is a char **
 * ((char **)response)[0] indicating PDP CID, which is generated by RIL. This Connection ID is
 *                          used in GSM/UMTS and CDMA
 * ((char **)response)[1] indicating the network interface name for GSM/UMTS or CDMA
 * ((char **)response)[2] indicating the IP address for this interface for GSM/UMTS
 *                          and NULL for CDMA
 *
 * FIXME may need way to configure QoS settings
 *
 * replaces  RIL_REQUEST_SETUP_DEFAULT_PDP
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DEACTIVATE_DATA_CALL
 */
#define RIL_REQUEST_SETUP_DATA_CALL 27



/**
 * RIL_REQUEST_SIM_IO
 *
 * Request SIM I/O operation.
 * This is similar to the TS 27.007 "restricted SIM" operation
 * where it assumes all of the EF selection will be done by the
 * callee.
 *
 * "data" is a const RIL_SIM_IO *
 * Please note that RIL_SIM_IO has a "PIN2" field which may be NULL,
 * or may specify a PIN2 for operations that require a PIN2 (eg
 * updating FDN records)
 *
 * "response" is a const RIL_SIM_IO_Response *
 *
 * Arguments and responses that are unused for certain
 * values of "command" should be ignored or set to NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  SIM_PIN2
 *  SIM_PUK2
 */
#define RIL_REQUEST_SIM_IO 28

/**
 * RIL_REQUEST_SEND_USSD
 *
 * Send a USSD message
 *
 * If a USSD session already exists, the message should be sent in the
 * context of that session. Otherwise, a new session should be created.
 *
 * The network reply should be reported via RIL_UNSOL_ON_USSD
 *
 * Only one USSD session may exist at a time, and the session is assumed
 * to exist until:
 *   a) The android system invokes RIL_REQUEST_CANCEL_USSD
 *   b) The implementation sends a RIL_UNSOL_ON_USSD with a type code
 *      of "0" (USSD-Notify/no further action) or "2" (session terminated)
 *
 * "data" is a const char * containing the USSD request in UTF-8 format
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  FDN_CHECK_FAILURE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_CANCEL_USSD, RIL_UNSOL_ON_USSD
 */

#define RIL_REQUEST_SEND_USSD 29

/**
 * RIL_REQUEST_CANCEL_USSD
 *
 * Cancel the current USSD session if one exists
 *
 * "data" is null
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_CANCEL_USSD 30

/**
 * RIL_REQUEST_GET_CLIR
 *
 * Gets current CLIR status
 * "data" is NULL
 * "response" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 * ((int *)data)[1] is "m" parameter from TS 27.007 7.7
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_CLIR 31

/**
 * RIL_REQUEST_SET_CLIR
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CLIR 32

/**
 * RIL_REQUEST_QUERY_CALL_FORWARD_STATUS
 *
 * "data" is const RIL_CallForwardInfo *
 *
 * "response" is const RIL_CallForwardInfo **
 * "response" points to an array of RIL_CallForwardInfo *'s, one for
 * each distinct registered phone number.
 *
 * For example, if data is forwarded to +18005551212 and voice is forwarded
 * to +18005559999, then two separate RIL_CallForwardInfo's should be returned
 *
 * If, however, both data and voice are forwarded to +18005551212, then
 * a single RIL_CallForwardInfo can be returned with the service class
 * set to "data + voice = 3")
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_CALL_FORWARD_STATUS 33


/**
 * RIL_REQUEST_SET_CALL_FORWARD
 *
 * Configure call forward rule
 *
 * "data" is const RIL_CallForwardInfo *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CALL_FORWARD 34


/**
 * RIL_REQUEST_QUERY_CALL_WAITING
 *
 * Query current call waiting state
 *
 * "data" is const int *
 * ((const int *)data)[0] is the TS 27.007 service class to query.
 * "response" is a const int *
 * ((const int *)response)[0] is 0 for "disabled" and 1 for "enabled"
 *
 * If ((const int *)response)[0] is = 1, then ((const int *)response)[1]
 * must follow, with the TS 27.007 service class bit vector of services
 * for which call waiting is enabled.
 *
 * For example, if ((const int *)response)[0]  is 1 and
 * ((const int *)response)[1] is 3, then call waiting is enabled for data
 * and voice and disabled for everything else
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_CALL_WAITING 35


/**
 * RIL_REQUEST_SET_CALL_WAITING
 *
 * Configure current call waiting state
 *
 * "data" is const int *
 * ((const int *)data)[0] is 0 for "disabled" and 1 for "enabled"
 * ((const int *)data)[1] is the TS 27.007 service class bit vector of
 *                           services to modify
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CALL_WAITING 36

/**
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * Acknowledge successful or failed receipt of SMS previously indicated
 * via RIL_UNSOL_RESPONSE_NEW_SMS
 *
 * "data" is int *
 * ((int *)data)[0] is 1 on successful receipt
 *                  (basically, AT+CNMA=1 from TS 27.005
 *                  is 0 on failed receipt
 *                  (basically, AT+CNMA=2 from TS 27.005)
 * ((int *)data)[1] if data[0] is 0, this contains the failure cause as defined
 *                  in TS 23.040, 9.2.3.22. Currently only 0xD3 (memory
 *                  capacity exceeded) and 0xFF (unspecified error) are
 *                  reported.
 *
 * "response" is NULL
 *
 * FIXME would like request that specified RP-ACK/RP-ERROR PDU
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SMS_ACKNOWLEDGE  37

/**
 * RIL_REQUEST_GET_IMEI - DEPRECATED
 *
 * Get the device IMEI, including check digit
 *
 * The request is DEPRECATED, use RIL_REQUEST_DEVICE_IDENTITY
 * Valid when RadioState is not RADIO_STATE_UNAVAILABLE
 *
 * "data" is NULL
 * "response" is a const char * containing the IMEI
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_IMEI 38

/**
 * RIL_REQUEST_GET_IMEISV - DEPRECATED
 *
 * Get the device IMEISV, which should be two decimal digits
 *
 * The request is DEPRECATED, use RIL_REQUEST_DEVICE_IDENTITY
 * Valid when RadioState is not RADIO_STATE_UNAVAILABLE
 *
 * "data" is NULL
 * "response" is a const char * containing the IMEISV
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_IMEISV 39


/**
 * RIL_REQUEST_ANSWER
 *
 * Answer incoming call
 *
 * Will not be called for WAITING calls.
 * RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE will be used in this case
 * instead
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_ANSWER 40

/**
 * RIL_REQUEST_DEACTIVATE_DATA_CALL
 *
 * Deactivate packet data connection
 * replaces RIL_REQUEST_DEACTIVATE_DEFAULT_PDP
 *
 * "data" is const char **
 * ((char**)data)[0] indicating CID
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SETUP_DATA_CALL
 */
#define RIL_REQUEST_DEACTIVATE_DATA_CALL 41

/**
 * RIL_REQUEST_QUERY_FACILITY_LOCK
 *
 * Query the status of a facility lock state
 *
 * "data" is const char **
 * ((const char **)data)[0] is the facility string code from TS 27.007 7.4
 *                      (eg "AO" for BAOC, "SC" for SIM lock)
 * ((const char **)data)[1] is the password, or "" if not required
 * ((const char **)data)[2] is the TS 27.007 service class bit vector of
 *                           services to query
 *
 * "response" is an int *
 * ((const int *)response) 0 is the TS 27.007 service class bit vector of
 *                           services for which the specified barring facility
 *                           is active. "0" means "disabled for all"
 *
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_QUERY_FACILITY_LOCK 42

/**
 * RIL_REQUEST_SET_FACILITY_LOCK
 *
 * Enable/disable one facility lock
 *
 * "data" is const char **
 *
 * ((const char **)data)[0] = facility string code from TS 27.007 7.4
 * (eg "AO" for BAOC)
 * ((const char **)data)[1] = "0" for "unlock" and "1" for "lock"
 * ((const char **)data)[2] = password
 * ((const char **)data)[3] = string representation of decimal TS 27.007
 *                            service class bit vector. Eg, the string
 *                            "1" means "set this facility for voice services"
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SET_FACILITY_LOCK 43

/**
 * RIL_REQUEST_CHANGE_BARRING_PASSWORD
 *
 * Change call barring facility password
 *
 * "data" is const char **
 *
 * ((const char **)data)[0] = facility string code from TS 27.007 7.4
 * (eg "AO" for BAOC)
 * ((const char **)data)[1] = old password
 * ((const char **)data)[2] = new password
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CHANGE_BARRING_PASSWORD 44

/**
 * RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE
 *
 * Query current network selectin mode
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((const int *)response)[0] is
 *     0 for automatic selection
 *     1 for manual selection
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE 45

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC
 *
 * Specify that the network should be selected automatically
 *
 * "data" is NULL
 * "response" is NULL
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  ILLEGAL_SIM_OR_ME
 *  GENERIC_FAILURE
 *
 * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
 *       no retries needed, such as illegal SIM or ME.
 *       Returns GENERIC_FAILURE for all other causes that might be
 *       fixed by retries.
 *
 */
#define RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC 46

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL
 *
 * Manually select a specified network.
 *
 * "data" is const char * specifying MCCMNC of network to select (eg "310170")
 * "response" is NULL
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  ILLEGAL_SIM_OR_ME
 *  GENERIC_FAILURE
 *
 * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
 *       no retries needed, such as illegal SIM or ME.
 *       Returns GENERIC_FAILURE for all other causes that might be
 *       fixed by retries.
 *
 */
#define RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL 47

/**
 * RIL_REQUEST_QUERY_AVAILABLE_NETWORKS
 *
 * Scans for available networks
 *
 * "data" is NULL
 * "response" is const char ** that should be an array of n*4 strings, where
 *    n is the number of available networks
 * For each available network:
 *
 * ((const char **)response)[n+0] is long alpha ONS or EONS
 * ((const char **)response)[n+1] is short alpha ONS or EONS
 * ((const char **)response)[n+2] is 5 or 6 digit numeric code (MCC + MNC)
 * ((const char **)response)[n+3] is a string value of the status:
 *           "unknown"
 *           "available"
 *           "current"
 *           "forbidden"
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_QUERY_AVAILABLE_NETWORKS 48

/**
 * RIL_REQUEST_DTMF_START
 *
 * Start playing a DTMF tone. Continue playing DTMF tone until
 * RIL_REQUEST_DTMF_STOP is received
 *
 * If a RIL_REQUEST_DTMF_START is received while a tone is currently playing,
 * it should cancel the previous tone and play the new one.
 *
 * "data" is a char *
 * ((char *)data)[0] is a single character with one of 12 values: 0-9,*,#
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DTMF, RIL_REQUEST_DTMF_STOP
 */
#define RIL_REQUEST_DTMF_START 49

/**
 * RIL_REQUEST_DTMF_STOP
 *
 * Stop playing a currently playing DTMF tone.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DTMF, RIL_REQUEST_DTMF_START
 */
#define RIL_REQUEST_DTMF_STOP 50

/**
 * RIL_REQUEST_BASEBAND_VERSION
 *
 * Return string value indicating baseband version, eg
 * response from AT+CGMR
 *
 * "data" is NULL
 * "response" is const char * containing version string for log reporting
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_BASEBAND_VERSION 51

/**
 * RIL_REQUEST_SEPARATE_CONNECTION
 *
 * Separate a party from a multiparty call placing the multiparty call
 * (less the specified party) on hold and leaving the specified party
 * as the only other member of the current (active) call
 *
 * Like AT+CHLD=2x
 *
 * See TS 22.084 1.3.8.2 (iii)
 * TS 22.030 6.5.5 "Entering "2X followed by send"
 * TS 27.007 "AT+CHLD=2x"
 *
 * "data" is an int *
 * (int *)data)[0] contains Connection index (value of 'x' in CHLD above) "response" is NULL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SEPARATE_CONNECTION 52


/**
 * RIL_REQUEST_SET_MUTE
 *
 * Turn on or off uplink (microphone) mute.
 *
 * Will only be sent while voice call is active.
 * Will always be reset to "disable mute" when a new voice call is initiated
 *
 * "data" is an int *
 * (int *)data)[0] is 1 for "enable mute" and 0 for "disable mute"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_SET_MUTE 53

/**
 * RIL_REQUEST_GET_MUTE
 *
 * Queries the current state of the uplink mute setting
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "mute enabled" and 0 for "mute disabled"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_MUTE 54

/**
 * RIL_REQUEST_QUERY_CLIP
 *
 * Queries the status of the CLIP supplementary service
 *
 * (for MMI code "*#30#")
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "CLIP provisioned"
 *                           and 0 for "CLIP not provisioned"
 *                           and 2 for "unknown, e.g. no network etc"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_QUERY_CLIP 55

/**
 * RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE
 *
 * Requests the failure cause code for the most recently failed PDP
 * context or CDMA data connection active
 * replaces RIL_REQUEST_LAST_PDP_FAIL_CAUSE
 *
 * "data" is NULL
 *
 * "response" is a "int *"
 * ((int *)response)[0] is an integer cause code defined in TS 24.008
 *   section 6.1.3.1.3 or close approximation
 *
 * If the implementation does not have access to the exact cause codes,
 * then it should return one of the values listed in
 * RIL_LastDataCallActivateFailCause, as the UI layer needs to distinguish these
 * cases for error notification
 * and potential retries.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_LAST_CALL_FAIL_CAUSE
 *
 */

#define RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE 56

/**
 * RIL_REQUEST_DATA_CALL_LIST
 *
 * Queries the status of PDP contexts, returning for each
 * its CID, whether or not it is active, and its PDP type,
 * APN, and PDP adddress.
 * replaces RIL_REQUEST_PDP_CONTEXT_LIST
 *
 * "data" is NULL
 * "response" is an array of RIL_Data_Call_Response
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_DATA_CALL_LIST 57

/**
 * RIL_REQUEST_RESET_RADIO - DEPRECATED
 *
 * Request a radio reset. The RIL implementation may postpone
 * the reset until after this request is responded to if the baseband
 * is presently busy.
 *
 * The request is DEPRECATED, use RIL_REQUEST_RADIO_POWER
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  REQUEST_NOT_SUPPORTED
 */

#define RIL_REQUEST_RESET_RADIO 58

/**
 * RIL_REQUEST_OEM_HOOK_RAW
 *
 * This request reserved for OEM-specific uses. It passes raw byte arrays
 * back and forth.
 *
 * It can be invoked on the Java side from
 * com.android.internal.telephony.Phone.invokeOemRilRequestRaw()
 *
 * "data" is a char * of bytes copied from the byte[] data argument in java
 * "response" is a char * of bytes that will returned via the
 * caller's "response" Message here:
 * (byte[])(((AsyncResult)response.obj).result)
 *
 * An error response here will result in
 * (((AsyncResult)response.obj).result) == null and
 * (((AsyncResult)response.obj).exception) being an instance of
 * com.android.internal.telephony.gsm.CommandException
 *
 * Valid errors:
 *  All
 */

#define RIL_REQUEST_OEM_HOOK_RAW 59

/**
 * RIL_REQUEST_OEM_HOOK_STRINGS
 *
 * This request reserved for OEM-specific uses. It passes strings
 * back and forth.
 *
 * It can be invoked on the Java side from
 * com.android.internal.telephony.Phone.invokeOemRilRequestStrings()
 *
 * "data" is a const char **, representing an array of null-terminated UTF-8
 * strings copied from the "String[] strings" argument to
 * invokeOemRilRequestStrings()
 *
 * "response" is a const char **, representing an array of null-terminated UTF-8
 * stings that will be returned via the caller's response message here:
 *
 * (String[])(((AsyncResult)response.obj).result)
 *
 * An error response here will result in
 * (((AsyncResult)response.obj).result) == null and
 * (((AsyncResult)response.obj).exception) being an instance of
 * com.android.internal.telephony.gsm.CommandException
 *
 * Valid errors:
 *  All
 */

#define RIL_REQUEST_OEM_HOOK_STRINGS 60

/**
 * RIL_REQUEST_SCREEN_STATE
 *
 * Indicates the current state of the screen.  When the screen is off, the
 * RIL should notify the baseband to suppress certain notifications (eg,
 * signal strength and changes in LAC/CID or BID/SID/NID/latitude/longitude)
 * in an effort to conserve power.  These notifications should resume when the
 * screen is on.
 *
 * "data" is int *
 * ((int *)data)[0] is == 1 for "Screen On"
 * ((int *)data)[0] is == 0 for "Screen Off"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SCREEN_STATE 61


/**
 * RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION
 *
 * Enables/disables supplementary service related notifications
 * from the network.
 *
 * Notifications are reported via RIL_UNSOL_SUPP_SVC_NOTIFICATION.
 *
 * "data" is int *
 * ((int *)data)[0] is == 1 for notifications enabled
 * ((int *)data)[0] is == 0 for notifications disabled
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_UNSOL_SUPP_SVC_NOTIFICATION.
 */
#define RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION 62

/**
 * RIL_REQUEST_WRITE_SMS_TO_SIM
 *
 * Stores a SMS message to SIM memory.
 *
 * "data" is RIL_SMS_WriteArgs *
 *
 * "response" is int *
 * ((const int *)response)[0] is the record index where the message is stored.
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_WRITE_SMS_TO_SIM 63

/**
 * RIL_REQUEST_DELETE_SMS_ON_SIM
 *
 * Deletes a SMS message from SIM memory.
 *
 * "data" is int  *
 * ((int *)data)[0] is the record index of the message to delete.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_DELETE_SMS_ON_SIM 64

/**
 * RIL_REQUEST_SET_BAND_MODE
 *
 * Assign a specified band for RF configuration.
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for "unspecified" (selected by baseband automatically)
 * ((int *)data)[0] is == 1 for "EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
 * ((int *)data)[0] is == 2 for "US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
 * ((int *)data)[0] is == 3 for "JPN band" (WCDMA-800 / WCDMA-IMT-2000)
 * ((int *)data)[0] is == 4 for "AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
 * ((int *)data)[0] is == 5 for "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
 * ((int *)data)[0] is == 6 for "Cellular (800-MHz Band)"
 * ((int *)data)[0] is == 7 for "PCS (1900-MHz Band)"
 * ((int *)data)[0] is == 8 for "Band Class 3 (JTACS Band)"
 * ((int *)data)[0] is == 9 for "Band Class 4 (Korean PCS Band)"
 * ((int *)data)[0] is == 10 for "Band Class 5 (450-MHz Band)"
 * ((int *)data)[0] is == 11 for "Band Class 6 (2-GMHz IMT2000 Band)"
 * ((int *)data)[0] is == 12 for "Band Class 7 (Upper 700-MHz Band)"
 * ((int *)data)[0] is == 13 for "Band Class 8 (1800-MHz Band)"
 * ((int *)data)[0] is == 14 for "Band Class 9 (900-MHz Band)"
 * ((int *)data)[0] is == 15 for "Band Class 10 (Secondary 800-MHz Band)"
 * ((int *)data)[0] is == 16 for "Band Class 11 (400-MHz European PAMR Band)"
 * ((int *)data)[0] is == 17 for "Band Class 15 (AWS Band)"
 * ((int *)data)[0] is == 18 for "Band Class 16 (US 2.5-GHz Band)"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_BAND_MODE 65

/**
 * RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE
 *
 * Query the list of band mode supported by RF.
 *
 * "data" is NULL
 *
 * "response" is int *
 * "response" points to an array of int's, the int[0] is the size of array, reset is one for
 * each available band mode.
 *
 *  0 for "unspecified" (selected by baseband automatically)
 *  1 for "EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
 *  2 for "US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
 *  3 for "JPN band" (WCDMA-800 / WCDMA-IMT-2000)
 *  4 for "AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
 *  5 for "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
 *  6 for "Cellular (800-MHz Band)"
 *  7 for "PCS (1900-MHz Band)"
 *  8 for "Band Class 3 (JTACS Band)"
 *  9 for "Band Class 4 (Korean PCS Band)"
 *  10 for "Band Class 5 (450-MHz Band)"
 *  11 for "Band Class 6 (2-GMHz IMT2000 Band)"
 *  12 for "Band Class 7 (Upper 700-MHz Band)"
 *  13 for "Band Class 8 (1800-MHz Band)"
 *  14 for "Band Class 9 (900-MHz Band)"
 *  15 for "Band Class 10 (Secondary 800-MHz Band)"
 *  16 for "Band Class 11 (400-MHz European PAMR Band)"
 *  17 for "Band Class 15 (AWS Band)"
 *  18 for "Band Class 16 (US 2.5-GHz Band)"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SET_BAND_MODE
 */
#define RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE 66

/**
 * RIL_REQUEST_STK_GET_PROFILE
 *
 * Requests the profile of SIM tool kit.
 * The profile indicates the SAT/USAT features supported by ME.
 * The SAT/USAT features refer to 3GPP TS 11.14 and 3GPP TS 31.111
 *
 * "data" is NULL
 *
 * "response" is a const char * containing SAT/USAT profile
 * in hexadecimal format string starting with first byte of terminal profile
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_GET_PROFILE 67

/**
 * RIL_REQUEST_STK_SET_PROFILE
 *
 * Download the STK terminal profile as part of SIM initialization
 * procedure
 *
 * "data" is a const char * containing SAT/USAT profile
 * in hexadecimal format string starting with first byte of terminal profile
 *
 * "response" is NULL
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SET_PROFILE 68

/**
 * RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND
 *
 * Requests to send a SAT/USAT envelope command to SIM.
 * The SAT/USAT envelope command refers to 3GPP TS 11.14 and 3GPP TS 31.111
 *
 * "data" is a const char * containing SAT/USAT command
 * in hexadecimal format string starting with command tag
 *
 * "response" is a const char * containing SAT/USAT response
 * in hexadecimal format string starting with first byte of response
 * (May be NULL)
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND 69

/**
 * RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE
 *
 * Requests to send a terminal response to SIM for a received
 * proactive command
 *
 * "data" is a const char * containing SAT/USAT response
 * in hexadecimal format string starting with first byte of response data
 *
 * "response" is NULL
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE 70

/**
 * RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM
 *
 * When STK application gets RIL_UNSOL_STK_CALL_SETUP, the call actually has
 * been initialized by ME already. (We could see the call has been in the 'call
 * list') So, STK application needs to accept/reject the call according as user
 * operations.
 *
 * "data" is int *
 * ((int *)data)[0] is > 0 for "accept" the call setup
 * ((int *)data)[0] is == 0 for "reject" the call setup
 *
 * "response" is NULL
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM 71

/**
 * RIL_REQUEST_EXPLICIT_CALL_TRANSFER
 *
 * Connects the two calls and disconnects the subscriber from both calls.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_EXPLICIT_CALL_TRANSFER 72

/**
 * RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE
 *
 * Requests to set the preferred network type for searching and registering
 * (CS/PS domain, RAT, and operation mode)
 *
 * "data" is int *
 *
 * ((int *)data)[0] is == 0 for GSM/WCDMA (WCDMA preferred)
 * ((int *)data)[0] is == 1 for GSM only
 * ((int *)data)[0] is == 2 for WCDMA only
 * ((int *)data)[0] is == 3 for GSM/WCDMA (auto mode, according to PRL)
 * ((int *)data)[0] is == 4 for CDMA and EvDo (auto mode, according to PRL)
 * ((int *)data)[0] is == 5 for CDMA only
 * ((int *)data)[0] is == 6 for EvDo only
 * ((int *)data)[0] is == 7 for GSM/WCDMA, CDMA, and EvDo (auto mode, according to PRL)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  MODE_NOT_SUPPORTED
 */
#define RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE 73

/**
 * RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE
 *
 * Query the preferred network type (CS/PS domain, RAT, and operation mode)
 * for searching and registering
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for GSM/WCDMA (WCDMA preferred)
 * ((int *)response)[0] is == 1 for GSM only
 * ((int *)response)[0] is == 2 for WCDMA only
 * ((int *)response)[0] is == 3 for GSM/WCDMA (auto mode, according to PRL)
 * ((int *)response)[0] is == 4 for CDMA and EvDo (auto mode, according to PRL)
 * ((int *)response)[0] is == 5 for CDMA only
 * ((int *)response)[0] is == 6 for EvDo only
 * ((int *)response)[0] is == 7 for GSM/WCDMA, CDMA, and EvDo (auto mode, according to PRL)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE
 */
#define RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE 74

/**
 * RIL_REQUEST_NEIGHBORING_CELL_IDS
 *
 * Request neighboring cell id in GSM network
 *
 * "data" is NULL
 * "response" must be a " const RIL_NeighboringCell** "
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_NEIGHBORING_CELL_IDS 75

/**
 * RIL_REQUEST_SET_LOCATION_UPDATES
 *
 * Enables/disables network state change notifications due to changes in
 * LAC and/or CID (for GSM) or BID/SID/NID/latitude/longitude (for CDMA).
 * Basically +CREG=2 vs. +CREG=1 (TS 27.007).
 *
 * Note:  The RIL implementation should default to "updates enabled"
 * when the screen is on and "updates disabled" when the screen is off.
 *
 * "data" is int *
 * ((int *)data)[0] is == 1 for updates enabled (+CREG=2)
 * ((int *)data)[0] is == 0 for updates disabled (+CREG=1)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SCREEN_STATE, RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED
 */
#define RIL_REQUEST_SET_LOCATION_UPDATES 76

/**
 * RIL_REQUEST_CDMA_SET_SUBSCRIPTION
 *
 * Request to set the location where the CDMA subscription shall
 * be retrieved
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 from RUIM/SIM (default)
 * ((int *)data)[0] is == 1 from NV
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  SIM_ABSENT
 *  SUBSCRIPTION_NOT_AVAILABLE
 */
#define RIL_REQUEST_CDMA_SET_SUBSCRIPTION 77

/**
 * RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE
 *
 * Request to set the roaming preferences in CDMA
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for Home Networks only, as defined in PRL
 * ((int *)data)[0] is == 1 for Roaming on Affiliated networks, as defined in PRL
 * ((int *)data)[0] is == 2 for Roaming on Any Network, as defined in the PRL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE 78

/**
 * RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE
 *
 * Request the actual setting of the roaming preferences in CDMA in the modem
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for Home Networks only, as defined in PRL
 * ((int *)response)[0] is == 1 for Roaming on Affiliated networks, as defined in PRL
 * ((int *)response)[0] is == 2 for Roaming on Any Network, as defined in the PRL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE 79

/**
 * RIL_REQUEST_SET_TTY_MODE
 *
 * Request to set the TTY mode
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for TTY off
 * ((int *)data)[0] is == 1 for TTY Full
 * ((int *)data)[0] is == 2 for TTY HCO (hearing carryover)
 * ((int *)data)[0] is == 3 for TTY VCO (voice carryover)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_TTY_MODE 80

/**
 * RIL_REQUEST_QUERY_TTY_MODE
 *
 * Request the setting of TTY mode
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for TTY off
 * ((int *)response)[0] is == 1 for TTY Full
 * ((int *)response)[0] is == 2 for TTY HCO (hearing carryover)
 * ((int *)response)[0] is == 3 for TTY VCO (voice carryover)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_TTY_MODE 81

/**
 * RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE
 *
 * Request to set the preferred voice privacy mode used in voice
 * scrambling
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for Standard Privacy Mode (Public Long Code Mask)
 * ((int *)data)[0] is == 1 for Enhanced Privacy Mode (Private Long Code Mask)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE 82

/**
 * RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE
 *
 * Request the setting of preferred voice privacy mode
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for Standard Privacy Mode (Public Long Code Mask)
 * ((int *)response)[0] is == 1 for Enhanced Privacy Mode (Private Long Code Mask)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE 83

/**
 * RIL_REQUEST_CDMA_FLASH
 *
 * Send FLASH
 *
 * "data" is const char *
 * ((const char *)data)[0] is a FLASH string
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_FLASH 84

/**
 * RIL_REQUEST_CDMA_BURST_DTMF
 *
 * Send DTMF string
 *
 * "data" is const char **
 * ((const char **)data)[0] is a DTMF string
 * ((const char **)data)[1] is the DTMF ON length in milliseconds, or 0 to use
 *                          default
 * ((const char **)data)[2] is the DTMF OFF length in milliseconds, or 0 to use
 *                          default
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_BURST_DTMF 85

/**
 * RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY
 *
 * Takes a 26 digit string (20 digit AKEY + 6 digit checksum).
 * If the checksum is valid the 20 digit AKEY is written to NV,
 * replacing the existing AKEY no matter what it was before.
 *
 * "data" is const char *
 * ((const char *)data)[0] is a 26 digit string (ASCII digits '0'-'9')
 *                         where the last 6 digits are a checksum of the
 *                         first 20, as specified in TR45.AHAG
 *                         "Common Cryptographic Algorithms, Revision D.1
 *                         Section 2.2"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY 86

/**
 * RIL_REQUEST_CDMA_SEND_SMS
 *
 * Send a CDMA SMS message
 *
 * "data" is const RIL_CDMA_SMS_Message *
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. The CDMA error class is derived as follows,
 * SUCCESS is error class 0 (no error)
 * SMS_SEND_FAIL_RETRY is error class 2 (temporary failure)
 * and GENERIC_FAILURE is error class 3 (permanent and no retry)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SEND_SMS 87

/**
 * RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE
 *
 * Acknowledge the success or failure in the receipt of SMS
 * previously indicated via RIL_UNSOL_RESPONSE_CDMA_NEW_SMS
 *
 * "data" is const RIL_CDMA_SMS_Ack *
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE 88

/**
 * RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG
 *
 * Request the setting of GSM/WCDMA Cell Broadcast SMS config.
 *
 * "data" is NULL
 *
 * "response" is a const RIL_GSM_BroadcastSmsConfigInfo **
 * "responselen" is count * sizeof (RIL_GSM_BroadcastSmsConfigInfo *)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG 89

/**
 * RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG
 *
 * Set GSM/WCDMA Cell Broadcast SMS config
 *
 * "data" is a const RIL_GSM_BroadcastSmsConfigInfo **
 * "datalen" is count * sizeof(RIL_GSM_BroadcastSmsConfigInfo *)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG 90

/**
 * RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION
 *
* Enable or disable the reception of GSM/WCDMA Cell Broadcast SMS
 *
 * "data" is const int *
 * (const int *)data[0] indicates to activate or turn off the
 * reception of GSM/WCDMA Cell Broadcast SMS, 0-1,
 *                       0 - Activate, 1 - Turn off
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION 91

/**
 * RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG
 *
 * Request the setting of CDMA Broadcast SMS config
 *
 * "data" is NULL
 *
 * "response" is a const RIL_CDMA_BroadcastSmsConfigInfo **
 * "responselen" is count * sizeof (RIL_CDMA_BroadcastSmsConfigInfo *)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG 92

/**
 * RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG
 *
 * Set CDMA Broadcast SMS config
 *
 * "data" is an const RIL_CDMA_BroadcastSmsConfigInfo **
 * "datalen" is count * sizeof(const RIL_CDMA_BroadcastSmsConfigInfo *)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG 93

/**
 * RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION
 *
 * Enable or disable the reception of CDMA Broadcast SMS
 *
 * "data" is const int *
 * (const int *)data[0] indicates to activate or turn off the
 * reception of CDMA Broadcast SMS, 0-1,
 *                       0 - Activate, 1 - Turn off
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION 94

/**
 * RIL_REQUEST_CDMA_SUBSCRIPTION
 *
 * Request the device MDN / H_SID / H_NID.
 *
 * The request is only allowed when CDMA subscription is available.  When CDMA
 * subscription is changed, application layer should re-issue the request to
 * update the subscription information.
 *
 * If a NULL value is returned for any of the device id, it means that error
 * accessing the device.
 *
 * "response" is const char **
 * ((const char **)response)[0] is MDN if CDMA subscription is available
 * ((const char **)response)[1] is a comma separated list of H_SID (Home SID) if
 *                              CDMA subscription is available, in decimal format
 * ((const char **)response)[2] is a comma separated list of H_NID (Home NID) if
 *                              CDMA subscription is available, in decimal format
 * ((const char **)response)[3] is MIN (10 digits, MIN2+MIN1) if CDMA subscription is available
 * ((const char **)response)[4] is PRL version if CDMA subscription is available
 *
 * Valid errors:
 *  SUCCESS
 *  RIL_E_SUBSCRIPTION_NOT_AVAILABLE
 */

#define RIL_REQUEST_CDMA_SUBSCRIPTION 95

/**
 * RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM
 *
 * Stores a CDMA SMS message to RUIM memory.
 *
 * "data" is RIL_CDMA_SMS_WriteArgs *
 *
 * "response" is int *
 * ((const int *)response)[0] is the record index where the message is stored.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM 96

/**
 * RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM
 *
 * Deletes a CDMA SMS message from RUIM memory.
 *
 * "data" is int  *
 * ((int *)data)[0] is the record index of the message to delete.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM 97

/**
 * RIL_REQUEST_DEVICE_IDENTITY
 *
 * Request the device ESN / MEID / IMEI / IMEISV.
 *
 * The request is always allowed and contains GSM and CDMA device identity;
 * it substitutes the deprecated requests RIL_REQUEST_GET_IMEI and
 * RIL_REQUEST_GET_IMEISV.
 *
 * If a NULL value is returned for any of the device id, it means that error
 * accessing the device.
 *
 * When CDMA subscription is changed the ESN/MEID may change.  The application
 * layer should re-issue the request to update the device identity in this case.
 *
 * "response" is const char **
 * ((const char **)response)[0] is IMEI if GSM subscription is available
 * ((const char **)response)[1] is IMEISV if GSM subscription is available
 * ((const char **)response)[2] is ESN if CDMA subscription is available
 * ((const char **)response)[3] is MEID if CDMA subscription is available
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DEVICE_IDENTITY 98

/**
 * RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE
 *
 * Request the radio's system selection module to exit emergency
 * callback mode.  RIL will not respond with SUCCESS until the modem has
 * completely exited from Emergency Callback Mode.
 *
 * "data" is NULL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE 99

/**
 * RIL_REQUEST_GET_SMSC_ADDRESS
 *
 * Queries the default Short Message Service Center address on the device.
 *
 * "data" is NULL
 *
 * "response" is const char * containing the SMSC address.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GET_SMSC_ADDRESS 100

/**
 * RIL_REQUEST_SET_SMSC_ADDRESS
 *
 * Sets the default Short Message Service Center address on the device.
 *
 * "data" is const char * containing the SMSC address.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SET_SMSC_ADDRESS 101

/**
 * RIL_REQUEST_REPORT_SMS_MEMORY_STATUS
 *
 * Indicates whether there is storage available for new SMS messages.
 *
 * "data" is int *
 * ((int *)data)[0] is 1 if memory is available for storing new messages
 *                  is 0 if memory capacity is exceeded
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_REPORT_SMS_MEMORY_STATUS 102

/**
 * RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING
 *
 * Indicates that the StkSerivce is running and is
 * ready to receive RIL_UNSOL_STK_XXXXX commands.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING 103

/**
 * RIL_REQUEST_GET_SERIAL_NUMBER - DEPRECATED
 *
 * Get the device SERIAL NUMBER, including check digit
 *
 * "data" is NULL
 * "response" is a const char * containing the SERIAL NUMBER
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_SERIAL_NUMBER 104
#define RIL_REQUEST_GET_MANUFACTURE_DATE_NUMBER 105 //kykim for ue_manufacture_date
#define RIL_REQUEST_GET_BARCODE_NUMBER 106 //kykim for ue_barcode_number

// onestar.kim 201107
#ifdef VIDEO_TELEPHONY_ENABLE
#define RIL_REQUEST_VT_BASE				RIL_REQUEST_GET_BARCODE_NUMBER
#define RIL_REQUEST_DIAL_VT				RIL_REQUEST_VT_BASE + 1
#define RIL_REQUEST_ANSWER_VT			RIL_REQUEST_VT_BASE + 2
#define RIL_REQUEST_HANGUP_VT			RIL_REQUEST_VT_BASE + 3
#define RIL_REQUEST_MODIFY_VT			RIL_REQUEST_VT_BASE + 4
#define RIL_REQUEST_CONFIRM_VT			RIL_REQUEST_VT_BASE + 5
#endif //VIDEO_TELEPHONY_ENABLE

/**
 * RIL_OEM_REQUEST_BASE
 *
 * SAMSUNG defined Ril Request.
 */

#define RIL_OEM_REQUEST_BASE 10000

#ifdef CellBroadcast
/** 
 * RIL_REQUEST_SET_CELL_BROADCAST_CONFIG

 * Request Cell Broadcast Configuration
 *
 * "data" is RIL_CB_ConfigArgs
 
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 * Contact : jiyoon.jeon@samsung.com
**/

#define RIL_REQUEST_SET_CELL_BROADCAST_CONFIG (RIL_OEM_REQUEST_BASE + 1)

/** 
 * RIL_REQUEST_GET_CELL_BROADCAST_CONFIG

 * Request Cell Broadcast Configuration
 *
 * "data" is NULL
 * "response" must be a " const RIL_CB_ConfigArgs** "
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  Contact : jiyoon.jeon@samsung.com
**/
#define RIL_REQUEST_GET_CELL_BROADCAST_CONFIG (RIL_OEM_REQUEST_BASE + 2)  

/** 
 * RIL_REQUEST_CRFM_LINE_SMS_COUNT_MSG

 * Request Confirm Line SMS count
 *
 * "data" is int  *
 * ((int *)data)[0] is the count of SMS.
 *
 * "response" is NULL
 *
 *  Contact : jiyoon.jeon@samsung.com
**/
#define RIL_REQUEST_CRFM_LINE_SMS_COUNT_MSG (RIL_OEM_REQUEST_BASE + 3)

/** 
 * RIL_REQUEST_CRFM_LINE_SMS_READ_MSG

 * Request Confirm Line Sms Read
 *
 * "data" is RIL_LineSmsReadMsg
 * "response" must be a " const RIL_LineSmsReadMsg** "
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  Contact : jiyoon.jeon@samsung.com
**/
#define RIL_REQUEST_CRFM_LINE_SMS_READ_MSG (RIL_OEM_REQUEST_BASE + 4)
#endif

/**
 * RIL_REQUEST_SEND_ENCODED_USSD
 *
 * Send a Encoded USSD message
 *
 * If a USSD session already exists, the message should be sent in the
 * context of that session. Otherwise, a new session should be created.
 *
 * The network reply should be reported via RIL_UNSOL_ON_USSD
 *
 * Only one USSD session may exist at a time, and the session is assumed
 * to exist until:
 *   a) The android system invokes RIL_REQUEST_CANCEL_USSD
 *   b) The implementation sends a RIL_UNSOL_ON_USSD with a type code
 *      of "0" (USSD-Notify/no further action) or "2" (session terminated)
 *
 * "data" is RIL_EncodedUSSD
 *  
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */

#define RIL_REQUEST_SEND_ENCODED_USSD (RIL_OEM_REQUEST_BASE + 5)

/** 
 * RIL_REQUEST_SET_PDA_MEMORY_STATUS
 *
 * set the PDA Memory Status
 *
 * "data" is int  *
 * ((int *)data)[0] is PDA MEMORY status
 *
 * 0x01= PDAMemory full
 * 0x02 = PD Memory Available
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 * 
 *Contact : seshu.t@samsung.com
**/

#define RIL_REQUEST_SET_PDA_MEMORY_STATUS (RIL_OEM_REQUEST_BASE + 6)

/** 
 * RIL_REQUEST_GET_PHONEBOOK_STORAGE_INFO
 *
 * get the phonebook storage informations
 *
 * "data" is int  *
 * ((int *)data)[0] is PDA MEMORY status
 *
 * 0x01= PDAMemory full
 * 0x02 = PD Memory Available
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 * 
 *Contact : lucky29.park@samsung.com
**/


#define RIL_REQUEST_GET_PHONEBOOK_STORAGE_INFO (RIL_OEM_REQUEST_BASE + 7) 
#define RIL_REQUEST_GET_PHONEBOOK_ENTRY (RIL_OEM_REQUEST_BASE + 8) 
#define RIL_REQUEST_ACCESS_PHONEBOOK_ENTRY (RIL_OEM_REQUEST_BASE + 9)

//[CONFIG_VT_SUPPORT
/** 
 * RIL_REQUEST_DIAL_VIDEO_CALL
 *
 * Initiate Video call 
 *
 * Valid errors:
 *  SUCCESS 
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DIAL_VIDEO_CALL (RIL_OEM_REQUEST_BASE + 10) 
//]

#define RIL_REQUEST_CALL_DEFLECTION (RIL_OEM_REQUEST_BASE + 11)

/**
 * RIL_REQUEST_READ_SMS_FROM_SIM
 *
 * read a SMS message from SIM memory.
 *
 * "data" is int  *
 * ((int *)data)[0] is the record index of the message to delete.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */

#define RIL_REQUEST_READ_SMS_FROM_SIM (RIL_OEM_REQUEST_BASE + 12)
#define RIL_REQUEST_USIM_PB_CAPA (RIL_OEM_REQUEST_BASE + 13)


/**
 * RIL_REQUEST_LOCK_INFO
 *
 * Request lock infomation
 *
 * "data" is int *
 * ((int *)data)[0] is Num of Lock Type
 * ((int *)data)[1] is Lock Type
 * "response" is const RIL_SIM_Lockinfo_Response
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */  
 
#define RIL_REQUEST_LOCK_INFO (RIL_OEM_REQUEST_BASE + 14)
/**
 * RIL_REQUEST_SEND_MOBILE_TRACKER_SMS
 *
 * Send an Mobile Tracker SMS message
 *
 * "data" is const char **
 * ((const char **)data)[0] is SMSC address in GSM BCD format prefixed
 *      by a length byte (as expected by TS 27.005) or NULL for default SMSC
 * ((const char **)data)[1] is SMS in PDU format as an ASCII hex string
 *      less the SMSC address
 *      TP-Layer-Length is be "strlen(((const char **)data)[1])/2"
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. SMS_SEND_FAIL_RETRY means retry (i.e. error cause is 332)
 * and GENERIC_FAILURE means no retry (i.e. error cause is 500)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  FDN_CHECK_FAILURE
 *  GENERIC_FAILURE
 *
 */

#define RIL_REQUEST_SEND_MOBILE_TRACKER_SMS (RIL_OEM_REQUEST_BASE + 15)

#define RIL_REQUEST_DIAL_EMERGENCY_CALL (RIL_OEM_REQUEST_BASE + 16)

#define RIL_REQUEST_GET_STOREAD_MSG_COUNT (RIL_OEM_REQUEST_BASE + 17)

#define RIL_REQUEST_STK_SIM_INIT_EVENT (RIL_OEM_REQUEST_BASE + 18)

/***********************************************************************/


#define RIL_UNSOL_RESPONSE_BASE 1000

/**
 * RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED
 *
 * Indicate when value of RIL_RadioState has changed.
 *
 * Callee will invoke RIL_RadioStateRequest method on main thread
 *
 * "data" is NULL
 */

#define RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED 1000


/**
 * RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED
 *
 * Indicate when call state has changed
 *
 * Callee will invoke RIL_REQUEST_GET_CURRENT_CALLS on main thread
 *
 * "data" is NULL
 *
 * Response should be invoked on, for example,
 * "RING", "BUSY", "NO CARRIER", and also call state
 * transitions (DIALING->ALERTING ALERTING->ACTIVE)
 *
 * Redundent or extraneous invocations are tolerated
 */
#define RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED 1001


/**
 * RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED
 *
 * Called when network state, operator name, or GPRS state has changed
 * Basically on, +CREG and +CGREG
 *
 * Callee will invoke the following requests on main thread:
 *
 * RIL_REQUEST_REGISTRATION_STATE
 * RIL_REQUEST_GPRS_REGISTRATION_STATE
 * RIL_REQUEST_OPERATOR
 *
 * "data" is NULL
 *
 * FIXME should this happen when SIM records are loaded? (eg, for
 * EONS)
 */
#define RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED 1002

/**
 * RIL_UNSOL_RESPONSE_NEW_SMS
 *
 * Called when new SMS is received.
 *
 * "data" is const char *
 * This is a pointer to a string containing the PDU of an SMS-DELIVER
 * as an ascii string of hex digits. The PDU starts with the SMSC address
 * per TS 27.005 (+CMT:)
 *
 * Callee will subsequently confirm the receipt of thei SMS with a
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * No new RIL_UNSOL_RESPONSE_NEW_SMS
 * or RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT messages should be sent until a
 * RIL_REQUEST_SMS_ACKNOWLEDGE has been received
 */

#define RIL_UNSOL_RESPONSE_NEW_SMS 1003

/**
 * RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT
 *
 * Called when new SMS Status Report is received.
 *
 * "data" is const char *
 * This is a pointer to a string containing the PDU of an SMS-STATUS-REPORT
 * as an ascii string of hex digits. The PDU starts with the SMSC address
 * per TS 27.005 (+CDS:).
 *
 * Callee will subsequently confirm the receipt of the SMS with a
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * No new RIL_UNSOL_RESPONSE_NEW_SMS
 * or RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT messages should be sent until a
 * RIL_REQUEST_SMS_ACKNOWLEDGE has been received
 */

#define RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT 1004

/**
 * RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM
 *
 * Called when new SMS has been stored on SIM card
 *
 * "data" is const int *
 * ((const int *)data)[0] contains the slot index on the SIM that contains
 * the new message
 */

#define RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM 1005

/**
 * RIL_UNSOL_ON_USSD
 *
 * Called when a new USSD message is received.
 *
 * "data" is const char **
 * ((const char **)data)[0] points to a type code, which is
 *  one of these string values:
 *      "0"   USSD-Notify -- text in ((const char **)data)[1]
 *      "1"   USSD-Request -- text in ((const char **)data)[1]
 *      "2"   Session terminated by network
 *      "3"   other local client (eg, SIM Toolkit) has responded
 *      "4"   Operation not supported
 *      "5"   Network timeout
 *
 * The USSD session is assumed to persist if the type code is "1", otherwise
 * the current session (if any) is assumed to have terminated.
 *
 * ((const char **)data)[1] points to a message string if applicable, which
 * should always be in UTF-8.
 */
#define RIL_UNSOL_ON_USSD 1006
/* Previously #define RIL_UNSOL_ON_USSD_NOTIFY 1006   */

/**
 * RIL_UNSOL_ON_USSD_REQUEST
 *
 * Obsolete. Send via RIL_UNSOL_ON_USSD
 */
#define RIL_UNSOL_ON_USSD_REQUEST 1007


/**
 * RIL_UNSOL_NITZ_TIME_RECEIVED
 *
 * Called when radio has received a NITZ time message
 *
 * "data" is const char * pointing to NITZ time string
 * in the form "yy/mm/dd,hh:mm:ss(+/-)tz,dt"
 */
#define RIL_UNSOL_NITZ_TIME_RECEIVED  1008

/**
 * RIL_UNSOL_SIGNAL_STRENGTH
 *
 * Radio may report signal strength rather han have it polled.
 *
 * "data" is a const RIL_SignalStrength *
 */
#define RIL_UNSOL_SIGNAL_STRENGTH  1009


/**
 * RIL_UNSOL_DATA_CALL_LIST_CHANGED
 *
 * Indicate a PDP context state has changed, or a new context
 * has been activated or deactivated
 * replaces RIL_UNSOL_PDP_CONTEXT_LIST_CHANGED
 *
 * "data" is an array of RIL_Data_Call_Response identical to that
 * returned by RIL_REQUEST_DATA_CALL_LIST
 *
 * See also: RIL_REQUEST_DATA_CALL_LIST
 */

#define RIL_UNSOL_DATA_CALL_LIST_CHANGED 1010

/**
 * RIL_UNSOL_SUPP_SVC_NOTIFICATION
 *
 * Reports supplementary service related notification from the network.
 *
 * "data" is a const RIL_SuppSvcNotification *
 *
 */

#define RIL_UNSOL_SUPP_SVC_NOTIFICATION 1011

/**
 * RIL_UNSOL_STK_SESSION_END
 *
 * Indicate when STK session is terminated by SIM.
 *
 * "data" is NULL
 */
#define RIL_UNSOL_STK_SESSION_END 1012

/**
 * RIL_UNSOL_STK_PROACTIVE_COMMAND
 *
 * Indicate when SIM issue a STK proactive command to applications
 *
 * "data" is a const char * containing SAT/USAT proactive command
 * in hexadecimal format string starting with command tag
 *
 */
#define RIL_UNSOL_STK_PROACTIVE_COMMAND 1013

/**
 * RIL_UNSOL_STK_EVENT_NOTIFY
 *
 * Indicate when SIM notifies applcations some event happens.
 * Generally, application does not need to have any feedback to
 * SIM but shall be able to indicate appropriate messages to users.
 *
 * "data" is a const char * containing SAT/USAT commands or responses
 * sent by ME to SIM or commands handled by ME, in hexadecimal format string
 * starting with first byte of response data or command tag
 *
 */
#define RIL_UNSOL_STK_EVENT_NOTIFY 1014

/**
 * RIL_UNSOL_STK_CALL_SETUP
 *
 * Indicate when SIM wants application to setup a voice call.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains timeout value (in milliseconds)
 */
#define RIL_UNSOL_STK_CALL_SETUP 1015

/**
 * RIL_UNSOL_SIM_SMS_STORAGE_FULL
 *
 * Indicates that SMS storage on the SIM is full.  Sent when the network
 * attempts to deliver a new SMS message.  Messages cannot be saved on the
 * SIM until space is freed.  In particular, incoming Class 2 messages
 * cannot be stored.
 *
 * "data" is null
 *
 */
#define RIL_UNSOL_SIM_SMS_STORAGE_FULL 1016

/**
 * RIL_UNSOL_SIM_REFRESH
 *
 * Indicates that file(s) on the SIM have been updated, or the SIM
 * has been reinitialized.
 *
 * "data" is an int *
 * ((int *)data)[0] is a RIL_SimRefreshResult.
 * ((int *)data)[1] is the EFID of the updated file if the result is
 * SIM_FILE_UPDATE or NULL for any other result.
 *
 * Note: If the radio state changes as a result of the SIM refresh (eg,
 * SIM_READY -> SIM_LOCKED_OR_ABSENT), RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED
 * should be sent.
 */
#define RIL_UNSOL_SIM_REFRESH 1017

/**
 * RIL_UNSOL_CALL_RING
 *
 * Ring indication for an incoming call (eg, RING or CRING event).
 * There must be at least one RIL_UNSOL_CALL_RING at the beginning
 * of a call and sending multiple is optional. If the system property
 * ro.telephony.call_ring.multiple is false then the upper layers
 * will generate the multiple events internally. Otherwise the vendor
 * ril must generate multiple RIL_UNSOL_CALL_RING if
 * ro.telephony.call_ring.multiple is true or if it is absent.
 *
 * The rate of these events is controlled by ro.telephony.call_ring.delay
 * and has a default value of 3000 (3 seconds) if absent.
 *
 * "data" is null for GSM
 * "data" is const RIL_CDMA_SignalInfoRecord * if CDMA
 */
#define RIL_UNSOL_CALL_RING 1018

/**
 * RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED
 *
 * Indicates that SIM state changes.
 *
 * Callee will invoke RIL_REQUEST_GET_SIM_STATUS on main thread

 * "data" is null
 */
#define RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED 1019

/**
 * RIL_UNSOL_RESPONSE_CDMA_NEW_SMS
 *
 * Called when new CDMA SMS is received
 *
 * "data" is const RIL_CDMA_SMS_Message *
 *
 * Callee will subsequently confirm the receipt of the SMS with
 * a RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE
 *
 * No new RIL_UNSOL_RESPONSE_CDMA_NEW_SMS should be sent until
 * RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE has been received
 *
 */
#define RIL_UNSOL_RESPONSE_CDMA_NEW_SMS 1020

/**
 * RIL_UNSOL_RESPONSE_NEW_BROADCAST_SMS
 *
 * Called when new Broadcast SMS is received
 *
 * "data" is const char * of 88 bytes which indicates each page
 * of a CBS Message sent to the MS by the BTS as coded in 3GPP
 * 23.041 Section 9.4.1.1
 *
 */
#define RIL_UNSOL_RESPONSE_NEW_BROADCAST_SMS 1021

/**
 * RIL_UNSOL_CDMA_RUIM_SMS_STORAGE_FULL
 *
 * Indicates that SMS storage on the RUIM is full.  Messages
 * cannot be saved on the RUIM until space is freed.
 *
 * "data" is null
 *
 */
#define RIL_UNSOL_CDMA_RUIM_SMS_STORAGE_FULL 1022

/**
 * RIL_UNSOL_RESTRICTED_STATE_CHANGED
 *
 * Indicates a restricted state change (eg, for Domain Specific Access Control).
 *
 * Radio need send this msg after radio off/on cycle no matter it is changed or not.
 *
 * "data" is an int *
 * ((int *)data)[0] contains a bitmask of RIL_RESTRICTED_STATE_* values.
 */
#define RIL_UNSOL_RESTRICTED_STATE_CHANGED 1023

/**
 * RIL_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE
 *
 * Indicates that the radio system selection module has
 * autonomously entered emergency callback mode.
 *
 * "data" is null
 *
 */
#define RIL_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE 1024

/**
 * RIL_UNSOL_CDMA_CALL_WAITING
 *
 * Called when CDMA radio receives a call waiting indication.
 *
 * "data" is const RIL_CDMA_CallWaiting *
 *
 */
#define RIL_UNSOL_CDMA_CALL_WAITING 1025

/**
 * RIL_UNSOL_CDMA_OTA_PROVISION_STATUS
 *
 * Called when CDMA radio receives an update of the progress of an
 * OTASP/OTAPA call.
 *
 * "data" is const int *
 *  For CDMA this is an integer OTASP/OTAPA status listed in
 *  RIL_CDMA_OTA_ProvisionStatus.
 *
 */
#define RIL_UNSOL_CDMA_OTA_PROVISION_STATUS 1026

/**
 * RIL_UNSOL_CDMA_INFO_REC
 *
 * Called when CDMA radio receives one or more info recs.
 *
 * "data" is const RIL_CDMA_InformationRecords *
 *
 */
#define RIL_UNSOL_CDMA_INFO_REC 1027

/**
 * RIL_UNSOL_OEM_HOOK_RAW
 *
 * This is for OEM specific use.
 *
 * "data" is a byte[]
 */
#define RIL_UNSOL_OEM_HOOK_RAW 1028

/**
 * RIL_UNSOL_RINGBACK_TONE
 *
 * Indicates that nework doesn't have in-band information,  need to
 * play out-band tone.
 *
 * "data" is an int *
 * ((int *)data)[0] == 0 for stop play ringback tone.
 * ((int *)data)[0] == 1 for start play ringback tone.
 */
#define RIL_UNSOL_RINGBACK_TONE 1029

/**
 * RIL_UNSOL_RESEND_INCALL_MUTE
 *
 * Indicates that framework/application need reset the uplink mute state.
 *
 * There may be situations where the mute state becomes out of sync
 * between the application and device in some GSM infrastructures.
 *
 * "data" is null
 */
#define RIL_UNSOL_RESEND_INCALL_MUTE 1030

#ifdef VIDEO_TELEPHONY_ENABLE
#define RIL_UNSOL_RESPONSE_VT_BASE									            RIL_UNSOL_RESEND_INCALL_MUTE
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_CALL_CONF					    RIL_UNSOL_RESPONSE_VT_BASE + 1
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_PROGRESS_INFO_IND			RIL_UNSOL_RESPONSE_VT_BASE + 2
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_CONNECT					      RIL_UNSOL_RESPONSE_VT_BASE + 3
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_SETUP_IND					    RIL_UNSOL_RESPONSE_VT_BASE + 4
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_INCOM						      RIL_UNSOL_RESPONSE_VT_BASE + 5
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_END						        RIL_UNSOL_RESPONSE_VT_BASE + 6
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_MODIFY_IND 				    RIL_UNSOL_RESPONSE_VT_BASE + 7
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_MODIFY_REQ 				    RIL_UNSOL_RESPONSE_VT_BASE + 8
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_MODIFY_COMPLETE_CONF		RIL_UNSOL_RESPONSE_VT_BASE + 9
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_MODIFY_RES 				    RIL_UNSOL_RESPONSE_VT_BASE + 10
#define RIL_UNSOL_RESPONSE_VT_CALL_EVENT_UNKNOWN 					      RIL_UNSOL_RESPONSE_VT_BASE + 11
#endif //VIDEO_TELEPHONY_ENABLE

typedef enum
{
    MN_SMS_UNASSIGNED_NUMBER = 1,
    MN_SMS_OP_DETERMINED_BARRING = 8,
    MN_SMS_CALL_BARRED = 10,
    MN_SMS_CP_NETWORK_FAILURE = 17,
    MN_SMS_TRANSFER_REJECTED = 21,
    MN_SMS_MEMORY_CAPACITY_EXCEEDED = 22,
    MN_SMS_DEST_OUT_OF_SERVICE = 27,
    MN_SMS_UNIDENTIFIED_SUBSCRIBER = 28,
    MN_SMS_FACILITY_REJECTED = 29,
    MN_SMS_UNKNOWN_SUBSCRIBER = 30,
    MN_SMS_NETWORK_OUT_OF_ORDER = 38,
    MN_SMS_TEMPORARY_FAILURE = 41,
    MN_SMS_CONGESTION = 42,
    MN_SMS_RESOURCES_UNAVAILABLE = 47,
    MN_SMS_FACILITY_NOT_SUBSCRIBED = 50,
    MN_SMS_REQ_FACILTY_NON_IMPL = 69,
    MN_SMS_INVALID_REFERENCE_VALUE = 81,
    MN_SMS_SEMANT_INCORRECT_MSG = 95,
    MN_SMS_INVALID_MANDATORY_INFO = 96,
    MN_SMS_MSG_TYPE_NON_EXISTENT = 97,
    MN_SMS_MSG_NOT_COMPATIBLE = 98,
    MN_SMS_IE_NON_EXISTENT = 99,
    MN_SMS_PROTOCOLL_ERROR = 111,
    MN_SMS_INTERWORKING = 127,

    TP_FCS_NO_ERROR = 0x00,
    TP_FCS_TELEMATIC_NOT_SUPPORTED = 0x80,
    TP_FCS_SM_TYPE_0_NOT_SUPPORTED = 0x81,
    TP_FCS_CANNOT_REPLACE_SM = 0x82,
    TP_FCS_UNSPEC_TP_PID_ERROR = 0x8F,
    TP_FCS_DCS_NOT_SUPPORTED = 0x90,
    TP_FCS_MSG_CLASS_NOT_SUPPORTED = 0x91,
    TP_FCS_UNSPEC_TP_DCS_ERROR = 0x9F,
    TP_FCS_CMD_NOT_ACTIONED = 0xA0,
    TP_FCS_CMD_NOT_SUPPORTED = 0xA1,
    TP_FCS_UNSPEC_TP_CMD_ERROR = 0xAF,
    TP_FCS_TPDU_NOT_SUPPORTED = 0xB0,
    TP_FCS_SC_BUSY = 0xC0,
    TP_FCS_NO_SC_SUBSCRIPTION = 0xC1,
    TP_FCS_SC_SYSTEM_FAILURE = 0xC2,
    TP_FCS_INVALID_SME_ADDRESS = 0xC3,
    TP_FCS_DEST_SME_BARRED = 0xC4,
    TP_FCS_SM_REJ_DUPL_SM = 0xC5,
    TP_FCS_SIM_SMS_STORE_FULL = 0xD0,
    TP_FCS_NO_SMS_ON_SIM = 0xD1,
    TP_FCS_ERROR_IN_MS = 0xD2,
    TP_FCS_MEM_CAP_EXCEEDED = 0xD3,
    TP_FCS_TOOLKIT_BUSY = 0xD4,
    TP_FCS_DATADOWNLOAD_ERROR = 0xD5,
    TP_FCS_APPL_ERR_START = 0xE0,
    TP_FCS_APPL_ERR_STOP = 0xFE,
    TP_FCS_UNSPECIFIED = 0xFF,

    MN_SMS_RP_ACK = 512,
    MN_SMS_TIMER_EXPIRED,
    MN_SMS_FORW_AVAIL_FAILED,
    MN_SMS_FORW_AVAIL_ABORTED,

    MN_TP_INVALID_MTI,
    MN_TP_SRF_NOT_IN_PHASE1,
    MN_TP_RDF_NOT_IN_PHASE1,
    MN_TP_RPF_NOT_IN_PHASE1,
    MN_TP_UDHF_NOT_IN_PHASE1,
    MN_TP_MISSING_VALIDITY_PERIOD,
    MN_TP_INVALID_TIME_STAMP,
    MN_TP_MISSING_DEST_ADDRESS,
    MN_TP_INVALID_DEST_ADDRESS,
    MN_TP_MISSING_SC_ADDRESS,
    MN_TP_INVALID_SC_ADDRESS,
    MN_TP_INVALID_ALPHABET,
    MN_TP_INVALID_USER_DATA_LENGTH,
    MN_TP_MISSING_USER_DATA,
    MN_TP_USER_DATA_TOO_LARGE,
    MN_TP_CMD_REQ_NOT_IN_PHASE1,
    MN_TP_INVALID_DEST_ADDR_SPEC_CMDS,
    MN_TP_INVALID_CMD_DATA_LENGTH,
    MN_TP_MISSING_CMD_DATA,
    MN_TP_INVALID_CMD_DATA_TYPE,
    MN_TP_CREATION_OF_MNR_FAILED,
    MN_TP_CREATION_OF_CMM_FAILED,
    MN_TP_MT_CONNECTION_LOST,
    MN_TP_PENDING_MO_SMS,
    MN_TP_CM_REJ_MSG_NOT_COMPAT,
    MN_SMS_REJ_BY_SMS_CONTROL,


    MN_SMS_NO_ERROR,
    MN_SMS_NO_ERROR_NO_ICON_DISPLAY,

    MN_SMS_FDN_FAILED,
    MN_SMS_BDN_FAILED,

    SMS_PP_UNSPECIFIED
} T_SMS_PP_CAUSE;

#define RIL_OEM_UNSOL_RESPONSE_BASE 11000

#ifdef CellBroadcast
/**
 * RIL_UNSOL_RESPONSE_NEW_CB_MSG
 *
 * Called when new CB is received.
 * 
 * "data" is const RIL_CBMsg_Notification
 * This is a pointer to a string containing the PDU of an GSM/UMTS Message Parameter
 * as an ascii string of hex digits. 
 * Contact @ : jiyoon.jeon@samsung.com
 */
#define RIL_UNSOL_RESPONSE_NEW_CB_MSG  (RIL_OEM_UNSOL_RESPONSE_BASE + 0) //add by jiyoon
#endif



/**
 * RIL_UNSOL_RELEASE_COMPLETE_MESSAGE
 *
 *
 *
 */
#define RIL_UNSOL_RELEASE_COMPLETE_MESSAGE	(RIL_OEM_UNSOL_RESPONSE_BASE + 1)

/*
 * RIL_UNSOL_STK_SEND_SMS_RESULT
 *
 * Indicate when send sms delivary notification to stk.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains Result of send sms 
Contact @ : suneel.diggi@samsung.com
*/
#define RIL_UNSOL_STK_SEND_SMS_RESULT (RIL_OEM_UNSOL_RESPONSE_BASE + 2)

/*
 * RIL_UNSOL_STK_CALL_CONTROL_RESULT
 *
 * Indicate when there is call control result to stk.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains Result of call control 
Contact @ : suneel.diggi@samsung.com
*/

#define RIL_UNSOL_STK_CALL_CONTROL_RESULT (RIL_OEM_UNSOL_RESPONSE_BASE + 3)

/*
 * RIL_UNSOL_DUN_CALL_STATUS
 *
 * Indicate when there is DUN call control result to platform.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains Result of DUN call control.
 */
#define RIL_UNSOL_DUN_CALL_STATUS (RIL_OEM_UNSOL_RESPONSE_BASE + 4)

/*
 * RIL_UNSOL_RESPONSE_LINE_SMS_COUNT
 *
 * Indicate when there is Line SMS Count Indication.
 *
 * "data" is null
Contact @ : jiyoon.jeon@samsung.com
*/
#define RIL_UNSOL_RESPONSE_LINE_SMS_COUNT (RIL_OEM_UNSOL_RESPONSE_BASE + 5)

/*
 * RIL_UNSOL_RESPONSE_LINE_SMS_COUNT
 *
 * Indicate when there is Line SMS Count Indication.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains index of SMS
 
Contact @ : jiyoon.jeon@samsung.com
 */

#define RIL_UNSOL_RESPONSE_LINE_SMS_READ  (RIL_OEM_UNSOL_RESPONSE_BASE + 6)

/*
 * RIL_UNSOL_O2_HOME_ZONE_INFO
 *
 * Indicate when there is O2 home zone information from modem.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains O2 home zone information.
*/

#define RIL_UNSOL_O2_HOME_ZONE_INFO (RIL_OEM_UNSOL_RESPONSE_BASE + 7)

/*
 * RIL_UNSOL_DEVICE_READY_NOTI
 *
 * Indicate to the Framework that device is ready.
 *
 * "data" is NULL
 *  Contact Seshu.t@samsung.com
 */
 
 #define RIL_UNSOL_DEVICE_READY_NOTI (RIL_OEM_UNSOL_RESPONSE_BASE + 8)

/* 
 * RIL_UNSOL_GPS_NOTI
 *
 * GPS-related unsolicited messages
 *
 * "data" is 
 * ((const *)data)[0] contains sub_request_id.
 * data + 1 is sub-request specific parameter.
 *
 */
#define RIL_UNSOL_GPS_NOTI	(RIL_OEM_UNSOL_RESPONSE_BASE + 9)

/* 
 * RIL_UNSOL_AM
 *
 * AM unsolicited messages
 *
 * "data" is 
 * ((const char **)data)[0] contains argument string to am 
 *
 */
#define RIL_UNSOL_AM	(RIL_OEM_UNSOL_RESPONSE_BASE + 10)

/* 
 * RIL_UNSOL_DUN_PIN_CONTROL_SIGNAL
 *
 * DUN PIN control unsolicited messages
 *
 * "data" is const int *
 * ((const int *)data)[0] contains PIN control signal where,
 *                 1: DTR
 *                 2: RTS(may not used)
 *                 3: DCD
 *                 4: DSR
 *                 5: CTS(may not used)
 *                 6: RI(may not used)
 * ((const int *)data)[1] contains PIN control signal status where,
 *                 0: OFF
 *                 1: ON
 */
#define RIL_UNSOL_DUN_PIN_CONTROL_SIGNAL (RIL_OEM_UNSOL_RESPONSE_BASE + 11)

/* 
 * RIL_UNSOL_DATA_SUSPEND_RESUME
 *
 * Data suspend/resume unsolicited messages
 *
 * "data" is const int *
 * ((const int *)data)[0] contains PIN control signal where,
 *                 0: SUSPEND
 *                 1: RESUME
 */
#define RIL_UNSOL_DATA_SUSPEND_RESUME (RIL_OEM_UNSOL_RESPONSE_BASE + 12)

/* 
 * RIL_UNSOL_SAP
 *
 * SAP unsolicited messages
 *
 * "data" is 
 * ((const *)data)[0] contains sub_request_id.
 * data + 1 is sub-request specific parameter.
 *                 
 *                 
 */
#define RIL_UNSOL_SAP (RIL_OEM_UNSOL_RESPONSE_BASE + 13)

#define RIL_UNSOL_RESPONSE_NO_NETWORK_RESPONSE (RIL_OEM_UNSOL_RESPONSE_BASE + 14)

#define RIL_UNSOL_SIM_SMS_STORAGE_AVAILALE (RIL_OEM_UNSOL_RESPONSE_BASE + 15)




/***********************************************************************/


/**
 * RIL_Request Function pointer
 *
 * @param request is one of RIL_REQUEST_*
 * @param data is pointer to data defined for that RIL_REQUEST_*
 *        data is owned by caller, and should not be modified or freed by callee
 * @param t should be used in subsequent call to RIL_onResponse
 * @param datalen the length of data
 *
 */
typedef void (*RIL_RequestFunc) (int request, void *data,
                                    size_t datalen, RIL_Token t);

/**
 * This function should return the current radio state synchronously
 */
typedef RIL_RadioState (*RIL_RadioStateRequest)();

/**
 * This function returns "1" if the specified RIL_REQUEST code is
 * supported and 0 if it is not
 *
 * @param requestCode is one of RIL_REQUEST codes
 */

typedef int (*RIL_Supports)(int requestCode);

/**
 * This function is called from a separate thread--not the
 * thread that calls RIL_RequestFunc--and indicates that a pending
 * request should be cancelled.
 *
 * On cancel, the callee should do its best to abandon the request and
 * call RIL_onRequestComplete with RIL_Errno CANCELLED at some later point.
 *
 * Subsequent calls to  RIL_onRequestComplete for this request with
 * other results will be tolerated but ignored. (That is, it is valid
 * to ignore the cancellation request)
 *
 * RIL_Cancel calls should return immediately, and not wait for cancellation
 *
 * Please see ITU v.250 5.6.1 for how one might implement this on a TS 27.007
 * interface
 *
 * @param t token wants to be canceled
 */

typedef void (*RIL_Cancel)(RIL_Token t);

typedef void (*RIL_TimedCallback) (void *param);

/**
 * Return a version string for your RIL implementation
 */
typedef const char * (*RIL_GetVersion) (void);

typedef struct {
    int version;        /* set to RIL_VERSION */
    RIL_RequestFunc onRequest;
    RIL_RadioStateRequest onStateRequest;
    RIL_Supports supports;
    RIL_Cancel onCancel;
    RIL_GetVersion getVersion;
} RIL_RadioFunctions;

#ifdef RIL_SHLIB
struct RIL_Env {
    /**
     * "t" is parameter passed in on previous call to RIL_Notification
     * routine.
     *
     * If "e" != SUCCESS, then response can be null/is ignored
     *
     * "response" is owned by caller, and should not be modified or
     * freed by callee
     *
     * RIL_onRequestComplete will return as soon as possible
     */
    void (*OnRequestComplete)(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

    /**
     * "unsolResponse" is one of RIL_UNSOL_RESPONSE_*
     * "data" is pointer to data defined for that RIL_UNSOL_RESPONSE_*
     *
     * "data" is owned by caller, and should not be modified or freed by callee
     */

    void (*OnUnsolicitedResponse)(int unsolResponse, const void *data,
                                    size_t datalen);

    /**
     * Call user-specifed "callback" function on on the same thread that
     * RIL_RequestFunc is called. If "relativeTime" is specified, then it specifies
     * a relative time value at which the callback is invoked. If relativeTime is
     * NULL or points to a 0-filled structure, the callback will be invoked as
     * soon as possible
     */

    void (*RequestTimedCallback) (RIL_TimedCallback callback,
                                   void *param, const struct timeval *relativeTime);
};


/**
 *  RIL implementations must defined RIL_Init
 *  argc and argv will be command line arguments intended for the RIL implementation
 *  Return NULL on error
 *
 * @param env is environment point defined as RIL_Env
 * @param argc number of arguments
 * @param argv list fo arguments
 *
 */
const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc, char **argv);

#else /* RIL_SHLIB */

/**
 * Call this once at startup to register notification routine
 *
 * @param callbacks user-specifed callback function
 */
void RIL_register (const RIL_RadioFunctions *callbacks);


/**
 *
 * RIL_onRequestComplete will return as soon as possible
 *
 * @param t is parameter passed in on previous call to RIL_Notification
 *          routine.
 * @param e error code
 *          if "e" != SUCCESS, then response can be null/is ignored
 * @param response is owned by caller, and should not be modified or
 *                 freed by callee
 * @param responselen the length of response in byte
 */
void RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

/**
 * @param unsolResponse is one of RIL_UNSOL_RESPONSE_*
 * @param data is pointer to data defined for that RIL_UNSOL_RESPONSE_*
 *     "data" is owned by caller, and should not be modified or freed by callee
 * @param datalen the length of data in byte
 */

void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen);


/**
 * Call user-specifed "callback" function on on the same thread that
 * RIL_RequestFunc is called. If "relativeTime" is specified, then it specifies
 * a relative time value at which the callback is invoked. If relativeTime is
 * NULL or points to a 0-filled structure, the callback will be invoked as
 * soon as possible
 *
 * @param callback user-specifed callback function
 * @param param parameter list
 * @param relativeTime a relative time value at which the callback is invoked
 */

void RIL_requestTimedCallback (RIL_TimedCallback callback,
                               void *param, const struct timeval *relativeTime);

/**
 * Remove user-specified callback function
 *  @param param  parameter list
 */

void RIL_removeTimedCallback(void *param);


#endif /* RIL_SHLIB */

#ifdef __cplusplus
}
#endif

#endif /*ANDROID_RIL_H*/
