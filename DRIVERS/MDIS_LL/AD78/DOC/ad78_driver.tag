<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
  </compound>
  <compound kind="file">
    <name>ad78_ctrl.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/AD78/TOOLS/AD78_CTRL/COM/</path>
    <filename>ad78__ctrl_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>NONE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_MAX_NBR</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintMdisError</name>
      <anchor>a7</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintUosError</name>
      <anchor>a8</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>ShowIo</name>
      <anchor>a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void __MAPILIB</type>
      <name>SigHandler</name>
      <anchor>a10</anchor>
      <arglist>(u_int32 sigCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a11</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a2</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>MDIS_PATH</type>
      <name>G_Path</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>u_int32</type>
      <name>G_SigCountSdEvt</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>u_int32</type>
      <name>G_SigCountOthers</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ad78_doc.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/AD78/DRIVER/COM/</path>
    <filename>ad78__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>ad78_drv.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/AD78/DRIVER/COM/</path>
    <filename>ad78__drv_8c</filename>
    <class kind="struct">LL_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>_NO_LL_HANDLE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_NUMBER</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_BYTES</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_IRQ</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_COUNT</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_SIZE</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEV_ID_AD78</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEV_ID_PU05</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEV_ID_PU06</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEV_ID_PU07</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEV_ID_PU08</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHAN_0</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHAN_1</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHAN_2</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBG_MYLEVEL</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBH</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMB_W_BYTE</name>
      <anchor>a16</anchor>
      <arglist>(cmd, val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMB_R_BYTE</name>
      <anchor>a17</anchor>
      <arglist>(cmd, valP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_Init</name>
      <anchor>a19</anchor>
      <arglist>(DESC_SPEC *descSpec, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_Exit</name>
      <anchor>a20</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_Read</name>
      <anchor>a21</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_Write</name>
      <anchor>a22</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_SetStat</name>
      <anchor>a23</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 code, int32 ch, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_GetStat</name>
      <anchor>a24</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 code, int32 ch, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_BlockRead</name>
      <anchor>a25</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_BlockWrite</name>
      <anchor>a26</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_Irq</name>
      <anchor>a27</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>AD78_Info</name>
      <anchor>a28</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a29</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a30</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>AlarmHandler</name>
      <anchor>a31</anchor>
      <arglist>(void *arg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>__AD78_GetEntry</name>
      <anchor>a32</anchor>
      <arglist>(LL_ENTRY *drvP)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a18</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ad78_drv.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>ad78__drv_8h</filename>
    <class kind="struct">AD78_BLK_DOWN_SIG</class>
    <member kind="define">
      <type>#define</type>
      <name>AD78_WOT</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_ONACK</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_ONACK_TOUT</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_ONACK_ERR</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_WDOG_ERR</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_SWOFF</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_OFFACK</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_DOWN_DELAY</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_OFF_DELAY</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_DOWN_EVT</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_DOWN_SIG_CLR</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_IN</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_IN_MASK</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_OUT</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_OUT_LOW</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_OUT_HIGH</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_OUT_MASK</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_TEMP</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_TEMP_HIGH</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_VOLTAGE</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_BLK_DOWN_SIG_SET</name>
      <anchor>a20</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_CURR</name>
      <anchor>a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_VOLT</name>
      <anchor>a22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_VOLT_LOW</name>
      <anchor>a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_VOLT_HIGH</name>
      <anchor>a24</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_CURR_LOW</name>
      <anchor>a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_CURR_HIGH</name>
      <anchor>a26</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_TEMP_ALERT</name>
      <anchor>a27</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05_POWER</name>
      <anchor>a28</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_VARIANT</name>
      <anchor>a29</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_AD78_GLOBNAME</name>
      <anchor>a30</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78_GLOBNAME</name>
      <anchor>a31</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AD78_GetEntry</name>
      <anchor>a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_OR_64</name>
      <anchor>a33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>U_INT32_OR_64</name>
      <anchor>a34</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>INT32_OR_64</type>
      <name>MDIS_PATH</name>
      <anchor>a35</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ad78_simp.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/AD78/EXAMPLE/AD78_SIMP/COM/</path>
    <filename>ad78__simp_8c</filename>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintError</name>
      <anchor>a0</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a1</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ad78_test.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/AD78/TOOLS/AD78_TEST/COM/</path>
    <filename>ad78__test_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>NONE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PWR_ON</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PWR_OFF</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSEC_2_MIN_SEC</name>
      <anchor>a3</anchor>
      <arglist>(msec, min, sec)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_START</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_GET_MIN_SEC</name>
      <anchor>a5</anchor>
      <arglist>(min, sec)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_GET_MSEC</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_GET_SEC</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FWARE_DELAY</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>POLL_TIME</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DELAYTIME</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USERTIME</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_TOUT_OFFSET</name>
      <anchor>a12</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOUT_OFFSET</name>
      <anchor>a13</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOUT_MIN</name>
      <anchor>a14</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOUT_MAX</name>
      <anchor>a15</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a19</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintMdisError</name>
      <anchor>a20</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>TestSwOffOn</name>
      <anchor>a21</anchor>
      <arglist>(u_int32 downDelay, u_int32 offDelay, u_int32 wot)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>Wait4DownEvent</name>
      <anchor>a22</anchor>
      <arglist>(u_int32 timeout)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>Wait4PwrOnOff</name>
      <anchor>a23</anchor>
      <arglist>(u_int32 state, u_int32 timeout)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>WotDuringDowndelay</name>
      <anchor>a24</anchor>
      <arglist>(u_int32 timeout)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>TestKeyOffOn</name>
      <anchor>a25</anchor>
      <arglist>(u_int32 downDelay, u_int32 offDelay)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>TestOffAck</name>
      <anchor>a26</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>TestOnAck</name>
      <anchor>a27</anchor>
      <arglist>(u_int32 testLevel)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>TestWdog</name>
      <anchor>a28</anchor>
      <arglist>(u_int32 wdog)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>CheckRstErroffErrcount</name>
      <anchor>a29</anchor>
      <arglist>(u_int32 *rst, u_int32 *errOff, u_int32 *ackErr, u_int32 *wdogErr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>Restart</name>
      <anchor>a30</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>WdogOnOff</name>
      <anchor>a31</anchor>
      <arglist>(u_int32 enable)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a32</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a16</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>MDIS_PATH</type>
      <name>G_Path</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>u_int32</type>
      <name>G_TimeStart</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ad78c.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>ad78c_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_FLAG</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WOT_L</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WOT_H</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_ERR</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_STATE</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_OFF</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_ON</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TRIG</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TOUT</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TOUT_MINMODE</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TOUT_MAXMODE</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_ERR</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_SWOFF</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFFACK</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_STATUS</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_STATUS_DOWN_EVT</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_IN</name>
      <anchor>a20</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_IN_MASK</name>
      <anchor>a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OUT</name>
      <anchor>a22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OUT_MASK</name>
      <anchor>a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP</name>
      <anchor>a24</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH</name>
      <anchor>a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_VOLT</name>
      <anchor>a26</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST1</name>
      <anchor>a27</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST1_NOTEST</name>
      <anchor>a28</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST1_TMODE1</name>
      <anchor>a29</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST2</name>
      <anchor>a30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST2_RST</name>
      <anchor>a31</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST2_SWON</name>
      <anchor>a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST3</name>
      <anchor>a33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST3_PWR</name>
      <anchor>a34</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST4</name>
      <anchor>a35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ID</name>
      <anchor>a36</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_REV</name>
      <anchor>a37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_SEC</name>
      <anchor>a0</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_SEC2MODE</name>
      <anchor>a1</anchor>
      <arglist>(sec, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_NONE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_MINMODE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_MAXMODE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MIN</name>
      <anchor>a0</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MIN2MODE</name>
      <anchor>a1</anchor>
      <arglist>(min, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MINMODE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MAXMODE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MIN</name>
      <anchor>a0</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MIN2MODE</name>
      <anchor>a1</anchor>
      <arglist>(min, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MINMODE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MAXMODE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ON_ACK_MAX_ERR</name>
      <anchor>a38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_MAX_ERR</name>
      <anchor>a39</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WAIT_OFF</name>
      <anchor>a40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ADC_REF_MV</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ADC_REF_VALUE</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ADC2INV</name>
      <anchor>a2</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_INV2ADC</name>
      <anchor>a3</anchor>
      <arglist>(mV)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_VOLT_FROM_INV</name>
      <anchor>a4</anchor>
      <arglist>(inVolt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_VOLT_FROM_ADC</name>
      <anchor>a5</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_FROM_INV</name>
      <anchor>a6</anchor>
      <arglist>(inVolt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_FROM_ADC</name>
      <anchor>a7</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_TO_INV</name>
      <anchor>a8</anchor>
      <arglist>(c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_TO_ADC</name>
      <anchor>a9</anchor>
      <arglist>(c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MIN_C</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MAX_C</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MIN</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MAX</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_DEFAULT</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>AD78C_SM_STATES</name>
      <anchor>a41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_OFF</name>
      <anchor>a41a69</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_SMB_ON_ACK</name>
      <anchor>a41a70</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_ON</name>
      <anchor>a41a71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_SHUTDOWN_DELAY</name>
      <anchor>a41a72</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_OFF_DELAY</name>
      <anchor>a41a73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_OFF</name>
      <anchor>a41a74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_ERROR_OFF</name>
      <anchor>a41a75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_RESET</name>
      <anchor>a41a76</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pu05_ctrl.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/AD78/TOOLS/PU05_CTRL/COM/</path>
    <filename>pu05__ctrl_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>NONE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHAN_MAX</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHK</name>
      <anchor>a2</anchor>
      <arglist>(expression, errst)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHK_CHAN</name>
      <anchor>a3</anchor>
      <arglist>(expression)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintMdisError</name>
      <anchor>a8</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>ShowIo</name>
      <anchor>a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>showInfo</name>
      <anchor>a10</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a11</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a4</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>MDIS_PATH</type>
      <name>G_Path</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>char *</type>
      <name>G_rail</name>
      <anchor>a6</anchor>
      <arglist>[3]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pu05c.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>pu05c_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>ADC_REF_MV</name>
      <anchor>a20</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADC_MAX_VAL</name>
      <anchor>a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADC2V</name>
      <anchor>a22</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_3V3_VOLT</name>
      <anchor>a23</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_5V_VOLT</name>
      <anchor>a24</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_12V_VOLT</name>
      <anchor>a25</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_3V3_CURR</name>
      <anchor>a26</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_5V_CURR</name>
      <anchor>a27</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_12V_CURR</name>
      <anchor>a28</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_TEMP_FROM_ADC</name>
      <anchor>a29</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_TO_INV</name>
      <anchor>a30</anchor>
      <arglist>(c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_TO_ADC</name>
      <anchor>a31</anchor>
      <arglist>(c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR3V3</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR5V</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR12V</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT3V3</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT5V</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT12V</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT3V3LO</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT3V3HI</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT5VLO</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT5VHI</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT12VLO</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT12VHI</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR3V3LO</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR3V3HI</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR5VLO</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR5VHI</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR12VLO</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR12VHI</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_TEMP_ALERT</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_POWER</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wdog.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>wdog_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_START</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TRIG</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_STOP</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_STATUS</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_SHOT</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_RESET_CTRL</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TRIG_PAT</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME_MIN</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME_MAX</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME_IRQ</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_OUT_PIN</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_OUT_REASON</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_PIN</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_SIGSET</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_SIGCLR</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_REASON</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_ERR_PIN</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TRIGPAT</name>
      <anchor>a19</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_CH_PROFILE_WDOG</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wdog_test.c</name>
    <path>/opt/menlinux/TOOLS/WDOG/WDOG_TEST/COM/</path>
    <filename>wdog__test_8c</filename>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintMdisError</name>
      <anchor>a2</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a3</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a0</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AD78_BLK_DOWN_SIG</name>
    <filename>structAD78__BLK__DOWN__SIG.html</filename>
    <member kind="variable">
      <type>u_int32</type>
      <name>msec</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>signal</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LL_HANDLE</name>
    <filename>structLL__HANDLE.html</filename>
    <member kind="variable">
      <type>int32</type>
      <name>memAlloc</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_HANDLE *</type>
      <name>osHdl</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_IRQ_HANDLE *</type>
      <name>irqHdl</name>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DESC_HANDLE *</type>
      <name>descHdl</name>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MDIS_IDENT_FUNCT_TBL</type>
      <name>idFuncTbl</name>
      <anchor>o4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>idCheck</name>
      <anchor>o5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>dbgLevel</name>
      <anchor>o6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBG_HANDLE *</type>
      <name>dbgHdl</name>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SMB_HANDLE *</type>
      <name>smbH</name>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int16</type>
      <name>smbAddr</name>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_SIG_HANDLE *</type>
      <name>sigHdl</name>
      <anchor>o10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_ALARM_HANDLE *</type>
      <name>alarmHdl</name>
      <anchor>o11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int8</type>
      <name>wdState</name>
      <anchor>o12</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>ad78dummy</name>
    <title>MEN logo</title>
    <filename>ad78dummy</filename>
  </compound>
  <compound kind="group">
    <name>_AD78C</name>
    <title>AD78 controller register and bit defines (cmd+byte access)</title>
    <filename>group____AD78C.html</filename>
    <subgroup>_AD78C_ONACK_TOUT</subgroup>
    <subgroup>_AD78C_DOWN_DELAY</subgroup>
    <subgroup>_AD78C_OFF_DELAY</subgroup>
    <subgroup>_AD78C_TEMP_VOLT</subgroup>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_FLAG</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WOT_L</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WOT_H</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_ERR</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_STATE</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_OFF</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_ON</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TRIG</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TOUT</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TOUT_MINMODE</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_TOUT_MAXMODE</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_ERR</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_SWOFF</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFFACK</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_STATUS</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_STATUS_DOWN_EVT</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_IN</name>
      <anchor>a20</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_IN_MASK</name>
      <anchor>a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OUT</name>
      <anchor>a22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OUT_MASK</name>
      <anchor>a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP</name>
      <anchor>a24</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH</name>
      <anchor>a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_VOLT</name>
      <anchor>a26</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST1</name>
      <anchor>a27</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST1_NOTEST</name>
      <anchor>a28</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST1_TMODE1</name>
      <anchor>a29</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST2</name>
      <anchor>a30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST2_RST</name>
      <anchor>a31</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST2_SWON</name>
      <anchor>a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST3</name>
      <anchor>a33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST3_PWR</name>
      <anchor>a34</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEST4</name>
      <anchor>a35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ID</name>
      <anchor>a36</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_REV</name>
      <anchor>a37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ON_ACK_MAX_ERR</name>
      <anchor>a38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WDOG_MAX_ERR</name>
      <anchor>a39</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_WAIT_OFF</name>
      <anchor>a40</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>AD78C_SM_STATES</name>
      <anchor>a41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_OFF</name>
      <anchor>a41a69</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_SMB_ON_ACK</name>
      <anchor>a41a70</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_ON</name>
      <anchor>a41a71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_SHUTDOWN_DELAY</name>
      <anchor>a41a72</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_OFF_DELAY</name>
      <anchor>a41a73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_WAIT_OFF</name>
      <anchor>a41a74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_ERROR_OFF</name>
      <anchor>a41a75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SM_STATE_RESET</name>
      <anchor>a41a76</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>_AD78C_ONACK_TOUT</name>
    <title>ON Acknowledge Timeout defines</title>
    <filename>group____AD78C__ONACK__TOUT.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_SEC</name>
      <anchor>a0</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_SEC2MODE</name>
      <anchor>a1</anchor>
      <arglist>(sec, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_NONE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_MINMODE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ONACK_TOUT_MAXMODE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>_AD78C_DOWN_DELAY</name>
    <title>Shutdown Delay defines</title>
    <filename>group____AD78C__DOWN__DELAY.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MIN</name>
      <anchor>a0</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MIN2MODE</name>
      <anchor>a1</anchor>
      <arglist>(min, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MINMODE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_DOWN_DELAY_MAXMODE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>_AD78C_OFF_DELAY</name>
    <title>OFF Delay defines</title>
    <filename>group____AD78C__OFF__DELAY.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MIN</name>
      <anchor>a0</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MIN2MODE</name>
      <anchor>a1</anchor>
      <arglist>(min, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MINMODE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_OFF_DELAY_MAXMODE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>_AD78C_TEMP_VOLT</name>
    <title>defines and macros for temperature and voltage computation</title>
    <filename>group____AD78C__TEMP__VOLT.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ADC_REF_MV</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ADC_REF_VALUE</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_ADC2INV</name>
      <anchor>a2</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_INV2ADC</name>
      <anchor>a3</anchor>
      <arglist>(mV)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_VOLT_FROM_INV</name>
      <anchor>a4</anchor>
      <arglist>(inVolt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_VOLT_FROM_ADC</name>
      <anchor>a5</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_FROM_INV</name>
      <anchor>a6</anchor>
      <arglist>(inVolt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_FROM_ADC</name>
      <anchor>a7</anchor>
      <arglist>(adc)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_TO_INV</name>
      <anchor>a8</anchor>
      <arglist>(c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_TO_ADC</name>
      <anchor>a9</anchor>
      <arglist>(c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MIN_C</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MAX_C</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MIN</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_MAX</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AD78C_TEMP_HIGH_DEFAULT</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>_PU05</name>
    <title>PU5 PIC Firmware Register defines (cmd+byte access)</title>
    <filename>group____PU05.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR3V3</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR5V</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR12V</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT3V3</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT5V</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT12V</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT3V3LO</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT3V3HI</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT5VLO</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT5VHI</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT12VLO</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_VOLT12VHI</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR3V3LO</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR3V3HI</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR5VLO</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR5VHI</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR12VLO</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_CURR12VHI</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_TEMP_ALERT</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PU05C_POWER</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
