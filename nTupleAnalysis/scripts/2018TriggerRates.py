# https://cmsoms.cern.ch/cms/triggers/l1_rates?cms_run=324980
# counts Pre DT before PS | Rate Hz  PreDT  before PS | pre DT counts after PS | rate pre DT after PS | counts post DT from HLT | rate post DT from HLT | initial PS | final PS
L1Info = {}

L1Info["L1_ZeroBias"          ] = (1564856894976 ,28590086.534  ,87514467      ,1598.898      ,84101295      ,1536.539      ,17881         ,17881         )
L1Info["L1_SingleJet35"       ] = (142350428064  ,2600756.063   ,2223074       ,40.616        ,2124188       ,38.809        ,0             ,64000         )
L1Info["L1_SingleJet60"       ] = (11236606019   ,205293.877    ,659797        ,12.055        ,630628        ,11.522        ,0             ,17000         )
L1Info["L1_SingleJet90"       ] = (1707957753    ,31204.553     ,2093845       ,38.255        ,2008497       ,36.695        ,0             ,375           )
L1Info["L1_SingleJet120"      ] = (458221249     ,8371.747      ,2416229       ,44.145        ,2317857       ,42.348        ,0             ,88            )
L1Info["L1_SingleJet180"      ] = (84974683      ,1552.496      ,84972359      ,1552.453      ,81235710      ,1484.184      ,0             ,1             )
L1Info["L1_SingleJet200"      ] = (60440929      ,1104.262      ,60439059      ,1104.227      ,57767380      ,1055.416      ,0             ,1             )
L1Info["L1_SingleJet35er2p5"  ] = (132567678145  ,2422024.277   ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_SingleJet60er2p5"  ] = (10188695876   ,186148.457    ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_SingleJet90er2p5"  ] = (1543579449    ,28201.346     ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_SingleJet120er2p5" ] = (415603545     ,7593.117      ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_SingleJet140er2p5" ] = (210624393     ,3848.128      ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_SingleJet160er2p5" ] = (120496488     ,2201.483      ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_SingleJet180er2p5" ] = (76215160      ,1392.458      ,76212962      ,1392.418      ,72862449      ,1331.204      ,0             ,1             )
L1Info["L1_HTT120er" ]          = (5684814700    ,103862.113    ,497478        ,9.089         ,474503        ,8.669         ,0             ,11400         )
L1Info["L1_HTT160er" ]          = (2047654950    ,37410.854     ,498278        ,9.104         ,475552        ,8.688         ,0             ,4100          )
L1Info["L1_HTT200er" ]          = (865957078     ,15821.119     ,540047        ,9.867         ,515606        ,9.420         ,0             ,1600          )
L1Info["L1_HTT255er" ]          = (327665930     ,5986.488      ,654164        ,11.952        ,624690        ,11.413        ,0             ,500           )
L1Info["L1_HTT280er" ]          = (225595595     ,4121.653      ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_HTT320er" ]          = (134102843     ,2450.072      ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_HTT360er" ]          = (86847001      ,1586.703      ,86845885      ,1586.683      ,82972786      ,1515.921      ,0             ,1             )
L1Info["L1_HTT400er" ]          = (60774141      ,1110.349      ,60773090      ,1110.330      ,58056371      ,1060.696      ,0             ,1             )
L1Info["L1_HTT450er" ]          = (42858029      ,783.020       ,42857022      ,783.002       ,40932287      ,747.837       ,0             ,1             )
L1Info["L1_ETT2000"  ]          = (21325980      ,389.628       ,21325039      ,389.611       ,20355238      ,371.892       ,0             ,1             )
L1Info["L1_SingleEG40er2p5" ]   = (302576740     ,5528.106      ,302572483     ,5528.029      ,289433757     ,5287.983      ,0             ,1             )
L1Info["L1_HTT280er_QuadJet_70_55_40_35_er2p4"] = (78778987    ,1439.300      ,0             ,0             ,0             ,0             ,0             ,0             )
L1Info["L1_HTT320er_QuadJet_70_55_40_40_er2p4"] = (42234655    ,771.631       ,42234655      ,771.631       ,40326190      ,736.763       ,0             ,1             )
L1Info["L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3"] = (36865061      ,673.528       ,36865061      ,673.528       ,35208665      ,643.266       ,0             ,1             )
L1Info["L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3"] = (28025023      ,512.020       ,28025023      ,512.020       ,26774071      ,489.165       ,0             ,1             )


debug = False
if debug:
    for l in L1Info:
        L1_PS = L1Info[l][7]
        if not L1_PS: continue
    
        rate_bPS_bDT = L1Info[l][1]
        rate_aPS_bDT = L1Info[l][3]
        rate_aPS_aDT = L1Info[l][5]
    
        est_PS_bDT = rate_bPS_bDT/rate_aPS_bDT
        est_PS_aDT = rate_bPS_bDT/rate_aPS_aDT
    
        
        #print l, "\t\t\trate",rate,"L1_PS=",L1_PS,"unprescaled rate",unprescaledrate
        print l, "L1_PS=",L1_PS,"vs",est_PS_bDT/L1_PS,"and",est_PS_aDT/L1_PS
    




# chain rate counts
# https://cmsoms.cern.ch/cms/triggers/hlt_rates/fullscreen/2332?cms_run=324980&update=false
hltInfo = {}



# name   =  L1 Pass | PS Pass |  Accepted | Rejected | Excepted | Rate

hltInfo["HLT_PFHT180" ] = ("L1_HTT120er", 474503          ,474503          ,41881           ,4066196927      ,0               ,0.768           )
hltInfo["HLT_PFHT250" ] = ("L1_HTT160er",475552          ,475552          ,36707           ,4066202101      ,0               ,0.673           )
#hltInfo["HLT_PFHT350" ] = ("L1_HTT360er",98312196        ,383543          ,154636          ,4066084172      ,0               ,2.836           )
hltInfo["HLT_PFHT370" ] = ("L1_HTT200er",515606          ,515606          ,21229           ,4066217579      ,0               ,0.389           )
hltInfo["HLT_PFHT430" ] = ("L1_HTT255er",624690      ,624690      ,36769       ,4066202039  ,0           ,0.674       )
# PS: L1_HTT120er OR L1_HTT160er OR L1_HTT200er OR L1_HTT255er OR 
# L1_HTT360er meanss : UNPS L1_HTT360er OR   L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3  L1_ETT2000 
hltInfo["HLT_PFHT510" ] = ("L1_HTT360er",97480045    ,380464      ,35446       ,4066203362  ,0           ,0.650       )
hltInfo["HLT_PFHT590" ] = ("L1_HTT360er",97480045    ,761455      ,38214       ,4066200594  ,0           ,0.701       )
hltInfo["HLT_PFHT680" ] = ("L1_HTT360er",97480045    ,1522963     ,40649       ,4066198159  ,0           ,0.746       )
hltInfo["HLT_PFHT780" ] = ("L1_HTT360er",97480045    ,3046189     ,43158       ,4066195650  ,0           ,0.792       )
hltInfo["HLT_PFHT890" ] = ("L1_HTT360er",97480045    ,6092596     ,45998       ,4066192810  ,0           ,0.844       )
hltInfo["HLT_PFHT1050"] = ("L1_HTT360er",97480045        ,97480045        ,324961          ,4065913847      ,0               ,5.960           )


# L1_SingleEG40er2p5 means UNPS : "L1_SingleEG40er2p5 OR L1_SingleJet180   L1_SingleTau120er2p1  "

hltInfo["HLT_PFHT350MinPFJet15"] = ("L1_SingleEG40er2p5",336504771   ,110170      ,16374       ,4066222434  ,0           ,0.300       )

# L1_HTT360er means UNPS:  L1_HTT360er  OR L1_ETT2000 OR L1_HTT320er_QuadJet_70_55_40_40_er2p4 OR L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 OR "

hltInfo["HLT_PFHT330PT30_QuadPFJet_75_60_45_40"] = ("L1_HTT360er", 98312196    ,767900      ,90027       ,4066148781  ,0           ,1.651       )
hltInfo["HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5"] = ("L1_HTT360er", 98312196    ,98312196    ,205333      ,4066033475  ,0           ,3.766       )

# L1_HTT360er means UNPS "L1_HTT360er L1_ETT2000 L1_HTT320er_QuadJet_70_55_40_40_er2p4 OR L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 OR L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3 OR L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5  "

hltInfo["HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"] = ("L1_HTT360er",135067249   ,135067249   ,257011      ,4065981797  ,0           ,4.714       )
hltInfo["HLT_PFHT400_FivePFJet_100_100_60_30_30"] = ("L1_HTT360er",135067249   ,3376767     ,58163       ,4066180645  ,0           ,1.067       )
hltInfo["HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"] = ("L1_HTT360er",135067249   ,135067249   ,357730      ,4065881078  ,0           ,6.561       )


hltInfo["HLT_PFJet15"]   = ("L1_ZeroBias", 83741760            ,0                   ,0                   ,4066238808          ,0                   ,0       )
hltInfo["HLT_PFJet25"]   = ("L1_ZeroBias",83741760            ,0                   ,0                   ,4066238808          ,0                   ,0       )
hltInfo["HLT_PFJet40"]   = ("L1_ZeroBias",83741760          ,5548346           ,92754             ,4066146054        ,0                 ,1.701             )
hltInfo["HLT_PFJet60"]   = ("L1_SingleJet35", 2124188    ,2124188    ,72542      ,4066166266 ,0          ,1.330      )
hltInfo["HLT_PFJet80"]   = ("L1_SingleJet60", 630628     ,630628     ,86102      ,4066152706 ,0          ,1.579      )
hltInfo["HLT_PFJet140"]  = ("L1_SingleJet90",2008497             ,1354589             ,120760              ,4066118048          ,0                   ,2.215   )
hltInfo["HLT_PFJet200"]  = ("L1_SingleJet120", 2317857             ,1557379             ,108376              ,4066130432          ,0                   ,1.988   )
hltInfo["HLT_PFJet260"]  = ("L1_SingleJet180", 81235710            ,634164              ,71961               ,4066166847          ,0                   ,1.320   )
hltInfo["HLT_PFJet320"]  = ("L1_SingleJet180",81235710            ,2538332             ,107031              ,4066131777          ,0                   ,1.963   )
hltInfo["HLT_PFJet400"]  = ("L1_SingleJet180",81235710          ,5076950           ,71381             ,4066167427        ,0                 ,1.309             )
hltInfo["HLT_PFJet450"]  = ("L1_SingleJet180",81235710          ,10154262          ,78425             ,4066160383        ,0                 ,1.438             )
hltInfo["HLT_PFJet500"]  = ("L1_SingleJet180",81235710           ,81235710           ,362948             ,4065875860         ,0                  ,6.657        )
hltInfo["HLT_PFJet550"]  = ("L1_SingleJet180",81235710           ,81235710           ,225228             ,4066013580         ,0                  ,4.131        )


hltInfo["HLT_DiPFJetAve40"]  = ("L1_ZeroBias", 83741760   ,6336005    ,41312      ,4066197496 ,0          ,0.758      )
hltInfo["HLT_DiPFJetAve60"]  = ("L1_ZeroBias", 83741760   ,25832752   ,34654      ,4066204154 ,0          ,0.636      )
hltInfo["HLT_DiPFJetAve80"]  = ("L1_SingleJet60", 630628     ,630628     ,40697      ,4066198111 ,0          ,0.746      )
hltInfo["HLT_DiPFJetAve140"] = ("L1_SingleJet90",2008497    ,2008497    ,88955      ,4066149853 ,0          ,1.632           )
hltInfo["HLT_DiPFJetAve200"] = ("L1_SingleJet120", 2317857    ,2317857    ,80724      ,4066158084 ,0          ,1.481           )
hltInfo["HLT_DiPFJetAve260"] = ("L1_SingleJet180",81235710   ,1046550    ,59439      ,4066179369 ,0          ,1.090           )
hltInfo["HLT_DiPFJetAve320"] = ("L1_SingleJet180",81235710   ,2792614    ,59502      ,4066179306 ,0          ,1.091      )
hltInfo["HLT_DiPFJetAve400"] = ("L1_SingleJet180",81235710   ,8376711    ,60291      ,4066178517 ,0          ,1.106      )
hltInfo["HLT_DiPFJetAve500"] = ("L1_SingleJet180",81235710   ,33508347   ,78610      ,4066160198 ,0          ,1.442      )

hltInfo["HLT_DiPFJetAve60_HFJEC"]  = ("L1_SingleJet35",85961653   ,85443165   ,28087      ,4066210721 ,0          ,0.515      )
hltInfo["HLT_DiPFJetAve80_HFJEC"]  = ("L1_SingleJet60",1570113    ,785206     ,34447      ,4066204361 ,0          ,0.632      )
hltInfo["HLT_DiPFJetAve100_HFJEC"] = ("L1_SingleJet90",3895137    ,3895137    ,123017     ,4066115791 ,0          ,2.256     )
hltInfo["HLT_DiPFJetAve160_HFJEC"] = ("L1_SingleJet120",2694114    ,2694114    ,45187      ,4066193621 ,0          ,0.829     )
hltInfo["HLT_DiPFJetAve220_HFJEC"] = ("L1_SingleJet180",81235710   ,16754117   ,131151     ,4066107657 ,0          ,2.405     )
hltInfo["HLT_DiPFJetAve300_HFJEC"] = ("L1_SingleJet180",81235710   ,81235710   ,89823      ,4066148985 ,0          ,1.647     )

#L1_SingleJet180 means UPS "L1_TripleJet_105_85_75_DoubleJet_85_75_er2p5 OR L1_SingleJet180"

hltInfo["HLT_QuadPFJet111_90_80_15"] = ("L1_SingleJet180", 104745069  ,204546     ,27747      ,4066211061 ,0          ,0.509                                   )
hltInfo["HLT_QuadPFJet98_83_71_15"]  = ("L1_SingleJet180", 81235710   ,0          ,0          ,4066238808 ,0          ,0                                        )
hltInfo["HLT_QuadPFJet105_88_76_15"] = ("L1_SingleJet180",117120951  ,228523     ,34504      ,4066204304 ,0          ,0.633                                   )
hltInfo["HLT_QuadPFJet103_88_75_15"] = ("L1_SingleJet180",135598290  ,529911     ,73861      ,4066164947 ,0          ,1.355                                   )





for r in [
    "HLT_PFJet15" ,"HLT_PFJet25" ,"HLT_PFJet40" ,"HLT_PFJet60" ,"HLT_PFJet80" ,"HLT_PFJet140","HLT_PFJet200","HLT_PFJet260","HLT_PFJet320","HLT_PFJet400","HLT_PFJet450","HLT_PFJet500","HLT_PFJet550",
    "HLT_PFHT180" ,"HLT_PFHT250" ,"HLT_PFHT370" ,"HLT_PFHT430" ,"HLT_PFHT510" ,"HLT_PFHT590" ,"HLT_PFHT680" ,"HLT_PFHT780" ,"HLT_PFHT890" ,"HLT_PFHT1050",
    "HLT_DiPFJetAve40" ,"HLT_DiPFJetAve60" ,"HLT_DiPFJetAve80" ,"HLT_DiPFJetAve140","HLT_DiPFJetAve200","HLT_DiPFJetAve260","HLT_DiPFJetAve320","HLT_DiPFJetAve400","HLT_DiPFJetAve500",
    "HLT_DiPFJetAve60_HFJEC" ,"HLT_DiPFJetAve80_HFJEC" ,"HLT_DiPFJetAve100_HFJEC","HLT_DiPFJetAve160_HFJEC","HLT_DiPFJetAve220_HFJEC","HLT_DiPFJetAve300_HFJEC",
    "HLT_QuadPFJet98_83_71_15"     ,"HLT_QuadPFJet103_88_75_15","HLT_QuadPFJet105_88_76_15","HLT_QuadPFJet111_90_80_15",
"HLT_PFHT350MinPFJet15",
"HLT_PFHT330PT30_QuadPFJet_75_60_45_40",
"HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5",
"HLT_PFHT400_FivePFJet_100_100_60_30_30",
"HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5",
"HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5",

    ]:
    if not hltInfo[r][2]: continue
    if not hltInfo[r][1]: continue
    PS = float(hltInfo[r][1]) / float(hltInfo[r][2])
    rate = hltInfo[r][6]
    #print PS, hltInfo[r][1]
    HLTunprescaledrate = rate*PS

    L1_Item = hltInfo[r][0]
    L1_PS = L1Info[L1_Item][1]/L1Info[L1_Item][3]

    unprescaledrate = rate*(PS*L1_PS)

    print r, "\t\t\trate",rate,"\tPS=",round(PS,1),"\tHLT unprescaled rate",round(HLTunprescaledrate,3),"\tL1_PS",round(L1_PS,1),"\tunprescaled rate",round(unprescaledrate,3)






#hltInfo["HLT_QuadPFJet98_83_71_15"] = ( 0, 0	       )
#hltInfo["HLT_QuadPFJet111_90_80_15"] = ( 0.51, 27747   ) 
#hltInfo["HLT_QuadPFJet105_88_76_15"] = ( 0.63, 34504   ) 
#hltInfo["HLT_QuadPFJet103_88_75_15"] = ( 1.35, 73861   )
#
#hltInfo["HLT_DiPFJetAve80"] = ( 0.75, 40697            )
#hltInfo["HLT_DiPFJetAve80_HFJEC"] = ( 0.63, 34447      )
#hltInfo["HLT_DiPFJetAve60"] = ( 0.64, 34654            )
#hltInfo["HLT_DiPFJetAve60_HFJEC"] = ( 0.52, 28087      )
#hltInfo["HLT_DiPFJetAve500"] = ( 1.44, 78610           )
#hltInfo["HLT_DiPFJetAve40"] = ( 0.76, 41312            )
#hltInfo["HLT_DiPFJetAve400"] = ( 1.11, 60291           )
#hltInfo["HLT_DiPFJetAve320"] = ( 1.09, 59502           )
#hltInfo["HLT_DiPFJetAve300_HFJEC"] = ( 1.65, 89823     )
#hltInfo["HLT_DiPFJetAve260"] = ( 1.09, 59439           )
#hltInfo["HLT_DiPFJetAve220_HFJEC"] = ( 2.41, 131151    )
#hltInfo["HLT_DiPFJetAve200"] = ( 1.48, 80724           )
#hltInfo["HLT_DiPFJetAve160_HFJEC"] = (0.83, 45187      )
#hltInfo["HLT_DiPFJetAve140"] = ( 1.63, 88955           )
#hltInfo["HLT_DiPFJetAve100_HFJEC"] = ( 2.26, 123017    )
#hltInfo["HLT_PFHT180"] = (0.77, 41881   )
#hltInfo["HLT_PFHT250"] = ( 0.67, 36707   )
#hltInfo["HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5"] = ( 3.77, 205333  )
#hltInfo["HLT_PFHT330PT30_QuadPFJet_75_60_45_40"] = ( 1.65, 90027   )
#hltInfo["HLT_PFHT350MinPFJet15"] = ( 0.30, 16374   )
#hltInfo["HLT_PFHT350"] = ( 2.84, 154636  )
#hltInfo["HLT_PFHT370"] = ( 0.39, 21229   )
#hltInfo["HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"] = ( 6.56, 357730  )
#hltInfo["HLT_PFHT400_FivePFJet_100_100_60_30_30"] = ( 1.07, 58163                           )
#hltInfo["HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"] = ( 4.71, 257011  )
#hltInfo["HLT_PFHT430"] = ( 0.67 ,36769   )
#hltInfo["HLT_PFHT510"] = ( 0.65 ,35446   )
#hltInfo["HLT_PFHT590"] = ( 0.70 ,38214   )
#hltInfo["HLT_PFHT680"] = ( 0.75 ,40649   )
#hltInfo["HLT_PFHT780"] = ( 0.79 ,43158   )
#hltInfo["HLT_PFHT890"] = ( 0.84 ,45998   )
#hltInfo["HLT_PFHT1050"] = ( 5.96, 324961 )
#
#hltInfo["HLT_PFJet15"] = ( 0 ,0          )
#hltInfo["HLT_PFJet25"] = ( 0 ,0          )
#hltInfo["HLT_PFJet40"] = ( 1.70 ,92754   )
#hltInfo["HLT_PFJet60"] = ( 1.33 ,72542   )
#hltInfo["HLT_PFJet80"] = ( 1.58 ,86102   )
#hltInfo["HLT_PFJet140"] = ( 2.21 ,120760 ) 
#hltInfo["HLT_PFJet200"] = ( 1.99 ,108376 ) 
#hltInfo["HLT_PFJet260"] = ( 1.32 ,71961  ) 
#hltInfo["HLT_PFJet320"] = ( 1.96 ,107031 ) 
#hltInfo["HLT_PFJet400"] = ( 1.31 ,71381  ) 
#hltInfo["HLT_PFJet450"] = ( 1.44 ,78425  ) 
#hltInfo["HLT_PFJet500"] = ( 6.66 ,362948 ) 
#hltInfo["HLT_PFJet550"] = ( 4.13 ,225228 ) 
