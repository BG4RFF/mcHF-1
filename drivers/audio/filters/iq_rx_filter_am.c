/*  -*-  mode: c; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4; coding: utf-8  -*-  */
/************************************************************************************
**                                                                                 **
**                               mcHF QRP Transceiver                              **
**                             K Atanassov - M0NKA 2014                            **
**                                                                                 **
**---------------------------------------------------------------------------------**
**                                                                                 **
**  File name:                                                                     **
**  Description:                                                                   **
**  Last Modified:                                                                 **
**  Licence:		GNU GPLv3                                                      **
************************************************************************************/

#include "filters.h"
/*
 	 * 89 tap FIR lowpass filter for AM demodulation
     * 10 kHz lowpass
     * Fc = 9.70 kHz
	 *
     * -6dB @ 10.0 kHz
     * -20dB @ 10.45 kHz
     * -40dB @ 10.8 kHz
     * -60dB > 11.13 kHz
     *
     * 20141205 by KA7OEI using Iowa Hills Hilbert Filter Designer
*/

const float iq_rx_am_10k_coeffs[Q_NUM_TAPS] =
{
    0.000117722483050164,
    -0.000190826378141535,
    -0.000316454605198105,
    0.000084665259804020,
    0.000534437033829599,
    0.000223520439161135,
    -0.000632342517282613,
    -0.000725725616097602,
    0.000428204065620224,
    0.001280215798621918,
    0.000218375537426833,
    -0.001601721502248780,
    -0.001292161293926300,
    0.001330681506383581,
    0.002528032564646596,
    -0.000185878610189881,
    -0.003399805802419520,
    -0.001839090604793820,
    0.003240267262336325,
    0.004316174080536414,
    -0.001493992133713610,
    -0.006356339576212520,
    -0.001960308149814000,
    0.006786095910800701,
    0.006537642652845357,
    -0.004528723876747330,
    -0.010862593712668900,
    -0.000897173759372233,
    0.012971399058043866,
    0.008921931792469471,
    -0.010804921591446800,
    -0.017688783704440600,
    0.002879095183285337,
    0.024147999555009812,
    0.011076579092093660,
    -0.024429930878634200,
    -0.029750803160671700,
    0.014143913162437944,
    0.050329661474116855,
    0.012584602054512364,
    -0.069061533914324600,
    -0.071583230484314000,
    0.082176152223611229,
    0.306148763236497423,
    0.413192420891035361,
    0.306148763236497423,
    0.082176152223611229,
    -0.071583230484314000,
    -0.069061533914324600,
    0.012584602054512364,
    0.050329661474116855,
    0.014143913162437944,
    -0.029750803160671700,
    -0.024429930878634200,
    0.011076579092093660,
    0.024147999555009812,
    0.002879095183285337,
    -0.017688783704440600,
    -0.010804921591446800,
    0.008921931792469471,
    0.012971399058043866,
    -0.000897173759372233,
    -0.010862593712668900,
    -0.004528723876747330,
    0.006537642652845357,
    0.006786095910800701,
    -0.001960308149814000,
    -0.006356339576212520,
    -0.001493992133713610,
    0.004316174080536414,
    0.003240267262336325,
    -0.001839090604793820,
    -0.003399805802419520,
    -0.000185878610189881,
    0.002528032564646596,
    0.001330681506383581,
    -0.001292161293926300,
    -0.001601721502248780,
    0.000218375537426833,
    0.001280215798621918,
    0.000428204065620224,
    -0.000725725616097602,
    -0.000632342517282613,
    0.000223520439161135,
    0.000534437033829599,
    0.000084665259804020,
    -0.000316454605198105,
    -0.000190826378141535,
    0.000117722483050164
};

/*
 	 * 89 tap FIR lowpass filter for AM demodulation
     * "2.3 kHz lowpass"
     *
     * NOTE:  Because the actual bandpass is TWICE the lowpass, and the fact that one must off-
     * tune the receiver slightly to avoid the "zero-Hz" hole in the center of the passband,
     * this filter is somewhat narrower than "2.3 kHz"
     *
     * Fc = 1.78 kHz
     *
     * -6dB @ 1.99 kHz
     * -20dB @ 2.55 kHz
     * -40dB @ 2.90 kHz
     * -60dB > 3.09 kHz
     *
     * 20141205 by KA7OEI using Iowa Hills Hilbert Filter Designer
*/

const float iq_rx_am_2k3_coeffs[Q_NUM_TAPS] =
{
    -0.000196778133038791,
    -0.000275755280074211,
    -0.000353984612861587,
    -0.000418690075048854,
    -0.000454047164276299,
    -0.000442413394049822,
    -0.000366184434389176,
    -0.000210191425343778,
    0.000035537284709122,
    0.000372915408768254,
    0.000793177355713582,
    0.001274879514193310,
    0.001782913716894173,
    0.002268882448091475,
    0.002673096205319169,
    0.002928317701073665,
    0.002965203927037432,
    0.002719199905849284,
    0.002138436401413266,
    0.001192000201349910,
    -0.000122197108217858,
    -0.001770806410417120,
    -0.003680540305986990,
    -0.005736552692955240,
    -0.007784289231932950,
    -0.009635139756152750,
    -0.011075900709763000,
    -0.011881696038148500,
    -0.011831641788347900,
    -0.010726209478865100,
    -0.008404982146732770,
    -0.004763336656982600,
    0.000233449848468894,
    0.006540069844680653,
    0.014026193505423940,
    0.022477415356206891,
    0.031602482869331178,
    0.041046636023995359,
    0.050410360909975617,
    0.059272371807803176,
    0.067215232024002855,
    0.073851744736956312,
    0.078850120995801118,
    0.081955978437540142,
    0.083009440825973363,
    0.081955978437540142,
    0.078850120995801118,
    0.073851744736956312,
    0.067215232024002855,
    0.059272371807803176,
    0.050410360909975617,
    0.041046636023995359,
    0.031602482869331178,
    0.022477415356206891,
    0.014026193505423940,
    0.006540069844680653,
    0.000233449848468894,
    -0.004763336656982600,
    -0.008404982146732770,
    -0.010726209478865100,
    -0.011831641788347900,
    -0.011881696038148500,
    -0.011075900709763000,
    -0.009635139756152750,
    -0.007784289231932950,
    -0.005736552692955240,
    -0.003680540305986990,
    -0.001770806410417120,
    -0.000122197108217858,
    0.001192000201349910,
    0.002138436401413266,
    0.002719199905849284,
    0.002965203927037432,
    0.002928317701073665,
    0.002673096205319169,
    0.002268882448091475,
    0.001782913716894173,
    0.001274879514193310,
    0.000793177355713582,
    0.000372915408768254,
    0.000035537284709122,
    -0.000210191425343778,
    -0.000366184434389176,
    -0.000442413394049822,
    -0.000454047164276299,
    -0.000418690075048854,
    -0.000353984612861587,
    -0.000275755280074211,
    -0.000196778133038791
};

/*
 	 * 89 tap FIR lowpass filter for AM demodulation
     * 3.6 kHz lowpass
     * Fc = 3.41 kHz
     *
     * -6dB @ 3.61 kHz
     * -20dB @ 4.15 kHz
     * -40dB @ 4.52 kHz
     * -60dB > 4.73 kHz
     *
     * 20141205 by KA7OEI using Iowa Hills Hilbert Filter Designer
*/

const float iq_rx_am_3k6_coeffs[Q_NUM_TAPS] =
{
    0.000194236006761998,
    0.000282693396615949,
    0.000320173787778202,
    0.000266033714965954,
    0.000094222201767759,
    -0.000191128727083006,
    -0.000544956187969324,
    -0.000880855390916938,
    -0.001084659388254350,
    -0.001042012424123070,
    -0.000675226060253367,
    0.000019767665763547,
    0.000948239863530163,
    0.001914701347780859,
    0.002651381830401708,
    0.002876822184755611,
    0.002374910694293840,
    0.001076052561405293,
    -0.000882983411525426,
    -0.003143324896504320,
    -0.005167612397018260,
    -0.006344612664735190,
    -0.006139510014581940,
    -0.004258448206719960,
    -0.000785356097214045,
    0.003751918439268481,
    0.008418658238655563,
    0.012024924259495225,
    0.013375473421118661,
    0.011573528596812616,
    0.006312844603313269,
    -0.001915215334099090,
    -0.011763413332218100,
    -0.021156533713089800,
    -0.027603877979735700,
    -0.028651610038411300,
    -0.022392825182070700,
    -0.007931207001952560,
    0.014305891795470664,
    0.042485370371458303,
    0.073554037962071023,
    0.103673924978429130,
    0.128835165244061745,
    0.145533526176055278,
    0.151381738212891748,
    0.145533526176055278,
    0.128835165244061745,
    0.103673924978429130,
    0.073554037962071023,
    0.042485370371458303,
    0.014305891795470664,
    -0.007931207001952560,
    -0.022392825182070700,
    -0.028651610038411300,
    -0.027603877979735700,
    -0.021156533713089800,
    -0.011763413332218100,
    -0.001915215334099090,
    0.006312844603313269,
    0.011573528596812616,
    0.013375473421118661,
    0.012024924259495225,
    0.008418658238655563,
    0.003751918439268481,
    -0.000785356097214045,
    -0.004258448206719960,
    -0.006139510014581940,
    -0.006344612664735190,
    -0.005167612397018260,
    -0.003143324896504320,
    -0.000882983411525426,
    0.001076052561405293,
    0.002374910694293840,
    0.002876822184755611,
    0.002651381830401708,
    0.001914701347780859,
    0.000948239863530163,
    0.000019767665763547,
    -0.000675226060253367,
    -0.001042012424123070,
    -0.001084659388254350,
    -0.000880855390916938,
    -0.000544956187969324,
    -0.000191128727083006,
    0.000094222201767759,
    0.000266033714965954,
    0.000320173787778202,
    0.000282693396615949,
    0.000194236006761998,
};

/* alternative lowpass filter for AM demodulation
 *
 * the problem with the 5k filter is that heterodynes that occur
 * because of the 5k carrier spacing in broadcast stations
 * therefore this alternative filter with a cutoff at 4k5
 * 89 tap FIR lowpass filter
 * 4k5 lowpass ParkMcClellan
 *
 * -6dB @ 4.5kHz
 * -24dB @ 5kHz
 * Fc = 4.32kHz (0.1800)
 * Window off, transition width 0.06
 *
 */
const float iq_rx_am_4k5_coeffs[Q_NUM_TAPS] =
{
    12.62505461280822420E-6,
    -0.001023781148748946,
    -0.001379392726010002,
    -0.001857298885889991,
    -0.002007930982712860,
    -0.001645764802591053,
    -729.0049453258671970E-6,
    580.3247418821165410E-6,
    0.001921187006321661,
    0.002821439596649512,
    0.002854663846093201,
    0.001821006157654806,
    -121.3991924356805900E-6,
    -0.002433909268373769,
    -0.004320619640730387,
    -0.004979702734199301,
    -0.003909641145391516,
    -0.001164372205549721,
    0.002556303856712675,
    0.006045165075114637,
    0.007947771772716222,
    0.007246305410655999,
    0.003707127946145705,
    -0.001883752703394752,
    -0.007825126886278979,
    -0.011964484776280015,
    -0.012414164965104595,
    -0.008297951368073063,
    -261.8431451219605610E-6,
    0.009476671671323595,
    0.017635986356801487,
    0.020847250650268035,
    0.016840077425071362,
    0.005464567575098569,
    -0.010818978674222298,
    -0.027219933295235161,
    -0.037686443762518306,
    -0.036521090442195647,
    -0.020119645677827265,
    0.011696759647048831,
    0.055173533570635397,
    0.103180886473356048,
    0.146733643399102748,
    0.177107579650033015,
    0.187997676685657006,
    0.177107579650033015,
    0.146733643399102748,
    0.103180886473356048,
    0.055173533570635397,
    0.011696759647048831,
    -0.020119645677827265,
    -0.036521090442195647,
    -0.037686443762518306,
    -0.027219933295235161,
    -0.010818978674222298,
    0.005464567575098569,
    0.016840077425071362,
    0.020847250650268035,
    0.017635986356801487,
    0.009476671671323595,
    -261.8431451219605610E-6,
    -0.008297951368073063,
    -0.012414164965104595,
    -0.011964484776280015,
    -0.007825126886278979,
    -0.001883752703394752,
    0.003707127946145705,
    0.007246305410655999,
    0.007947771772716222,
    0.006045165075114637,
    0.002556303856712675,
    -0.001164372205549721,
    -0.003909641145391516,
    -0.004979702734199301,
    -0.004320619640730387,
    -0.002433909268373769,
    -121.3991924356805900E-6,
    0.001821006157654806,
    0.002854663846093201,
    0.002821439596649512,
    0.001921187006321661,
    580.3247418821165410E-6,
    -729.0049453258671970E-6,
    -0.001645764802591053,
    -0.002007930982712860,
    -0.001857298885889991,
    -0.001379392726010002,
    -0.001023781148748946,
    12.62505461280822420E-6
};



/*
 	 * 89 tap FIR lowpass filter for AM demodulation
     * 5 kHz lowpass
     * Fc = 2.30 kHz (0.096)
     * BW = 5.09 kHz (0.212)
     * Phase = 0.0 Deg
	 *
     * -6dB @ 5.0 kHz
     * -20dB @ 5.6 kHz
     * -40dB @ 6.0 kHz
     * -60dB > 6.2 kHz
     *
     * 20150724 by KA7OEI using Iowa Hills Hilbert Filter Designer
     *
*/

const float iq_rx_am_5k_coeffs[Q_NUM_TAPS] =
{
    -0.000045392289047881,
    -0.000004812442917794,
    0.000068727674699516,
    0.000141963927224663,
    0.000157520248119967,
    0.000060286967439699,
    -0.000162970122246712,
    -0.000451252675540791,
    -0.000669397068852781,
    -0.000655786056558042,
    -0.000308946001031250,
    0.000323862987296878,
    0.001009151276280870,
    0.001383767648753940,
    0.001108841899531180,
    0.000068694704026995,
    -0.001482086048715270,
    -0.002923873990383000,
    -0.003478042196892240,
    -0.002567012208450240,
    -0.000183058152703148,
    0.002927838581731980,
    0.005419014927265250,
    0.005850957020138160,
    0.003392366775186750,
    -0.001589789749145610,
    -0.007378579320778290,
    -0.011384168739919100,
    -0.011175196307323000,
    -0.005700279233064870,
    0.003842132441622350,
    0.013983598060774900,
    0.020024993830604200,
    0.017901468392382700,
    0.006217999161870360,
    -0.012402093620729800,
    -0.031370988485890300,
    -0.041832006141384800,
    -0.035588597131400500,
    -0.008295098525339310,
    0.038292174335735300,
    0.095971206103503100,
    0.151982896902598000,
    0.192635286648254000,
    0.207485090950182000,
    0.192635286648256000,
    0.151982896902602000,
    0.095971206103507900,
    0.038292174335739900,
    -0.008295098525336090,
    -0.035588597131399000,
    -0.041832006141385000,
    -0.031370988485891600,
    -0.012402093620731500,
    0.006217999161868930,
    0.017901468392382000,
    0.020024993830604300,
    0.013983598060775600,
    0.003842132441623320,
    -0.005700279233064020,
    -0.011175196307322600,
    -0.011384168739919100,
    -0.007378579320778620,
    -0.001589789749146100,
    0.003392366775186310,
    0.005850957020137900,
    0.005419014927265200,
    0.002927838581732100,
    -0.000183058152702952,
    -0.002567012208450050,
    -0.003478042196892130,
    -0.002923873990382970,
    -0.001482086048715290,
    0.000068694704026947,
    0.001108841899531140,
    0.001383767648753930,
    0.001009151276280870,
    0.000323862987296896,
    -0.000308946001031235,
    -0.000655786056558038,
    -0.000669397068852788,
    -0.000451252675540803,
    -0.000162970122246723,
    0.000060286967439694,
    0.000157520248119968,
    0.000141963927224668,
    0.000068727674699521,
    -0.000004812442917792,
    -0.000045392289047882
};

/*
 	 * 89 tap FIR lowpass filter for AM demodulation
     * 6 kHz lowpass
     * Fc = 2.64 kHz (0.110)
     * BW = 638 kHz (0.266)
     * Phase = 0.0 Deg
	 *
     * -6dB @ 6.0 kHz
     * -20dB @ 6.5 kHz
     * -40dB @ 7.0 kHz
     * -60dB > 7.2 kHz
     *
     * 20150724 by KA7OEI using Iowa Hills Hilbert Filter Designer
     *
*/

const float iq_rx_am_6k_coeffs[Q_NUM_TAPS] =
{
    -0.000004880093665016,
    0.000056249601206470,
    0.000106631842609010,
    0.000081384254139970,
    -0.000058736957755522,
    -0.000272365414638255,
    -0.000425548359876376,
    -0.000357422252818966,
    -0.000005244245277373,
    0.000490974654661712,
    0.000803539254316524,
    0.000594128178328428,
    -0.000221469661023948,
    -0.001297764644188940,
    -0.001948810489192100,
    -0.001535669576006110,
    0.000035159562865781,
    0.002042237502232450,
    0.003191717421535510,
    0.002364285159575850,
    -0.000519165742550591,
    -0.004107368177389280,
    -0.006132446052393760,
    -0.004729819141807280,
    0.000138536384265303,
    0.006130339738902320,
    0.009466230734482630,
    0.007122248925836100,
    -0.000902500392804507,
    -0.010767353229646000,
    -0.016344468781221300,
    -0.012706451532127600,
    0.000277710569700882,
    0.016562752996003900,
    0.026194213119574000,
    0.020664397226820300,
    -0.001236820922091710,
    -0.030498087727862400,
    -0.050224694289173800,
    -0.042930930378686000,
    0.000379604033916306,
    0.073954541623616600,
    0.157507699515675000,
    0.223513877747226000,
    0.248565464214855000,
    0.223513877747230000,
    0.157507699515682000,
    0.073954541623623400,
    0.000379604033921383,
    -0.042930930378683800,
    -0.050224694289174300,
    -0.030498087727864600,
    -0.001236820922093990,
    0.020664397226819100,
    0.026194213119574100,
    0.016562752996005000,
    0.000277710569702140,
    -0.012706451532126900,
    -0.016344468781221300,
    -0.010767353229646500,
    -0.000902500392805126,
    0.007122248925835810,
    0.009466230734482780,
    0.006130339738902770,
    0.000138536384265717,
    -0.004729819141807150,
    -0.006132446052393960,
    -0.004107368177389690,
    -0.000519165742550966,
    0.002364285159575700,
    0.003191717421535620,
    0.002042237502232730,
    0.000035159562866079,
    -0.001535669576005930,
    -0.001948810489192080,
    -0.001297764644189040,
    -0.000221469661024093,
    0.000594128178328318,
    0.000803539254316486,
    0.000490974654661737,
    -0.000005244245277321,
    -0.000357422252818922,
    -0.000425548359876357,
    -0.000272365414638260,
    -0.000058736957755538,
    0.000081384254139956,
    0.000106631842609003,
    0.000056249601206470,
    -0.000004880093665013
};


/*
 	 * 89 tap FIR lowpass filter for AM demodulation
     * 7.5 kHz lowpass
     * Fc = 3.50 kHz (0.146)
     * BW = 7.73 kHz (0.322)
     * Phase = 0.0 Deg
	 *
     * -6dB @ 7.5 kHz
     * -20dB @ 8.1 kHz
     * -40dB @ 8.5 kHz
     * -60dB > 8.7 kHz
     *
     * 20150724 by KA7OEI using Iowa Hills Hilbert Filter Designer

*/

const float iq_rx_am_7k5_coeffs[Q_NUM_TAPS] =
{
    -0.000054368878976464,
    -0.000109470418324297,
    -0.000080800014418553,
    0.000057887247193655,
    0.000196248384784124,
    0.000143919934167119,
    -0.000166720581170554,
    -0.000516949697820653,
    -0.000513457595923984,
    0.000019591792777547,
    0.000715438143813412,
    0.000837678462105361,
    -0.000026672117250676,
    -0.001346407587573860,
    -0.001854545301167850,
    -0.000681925402542360,
    0.001516983112219140,
    0.002720057594104930,
    0.001264577750721170,
    -0.002218475374737490,
    -0.004716082694953130,
    -0.003290070348926000,
    0.001768705442798720,
    0.006257195418023830,
    0.005315470630396060,
    -0.001674768165904550,
    -0.009231015015775820,
    -0.009741273892250180,
    -0.000735938093829474,
    0.011162261151525300,
    0.014549180415936700,
    0.003582582016681460,
    -0.014560284217669600,
    -0.023377317298097500,
    -0.011097551616034700,
    0.016127337734989000,
    0.035061508247232000,
    0.023170836448327200,
    -0.018977720268267700,
    -0.060089752963952400,
    -0.055598189384657500,
    0.019293047057938800,
    0.145271104740461000,
    0.264027404678331000,
    0.312598578987250000,
    0.264027404678338000,
    0.145271104740472000,
    0.019293047057947600,
    -0.055598189384654300,
    -0.060089752963954300,
    -0.018977720268271600,
    0.023170836448324900,
    0.035061508247232500,
    0.016127337734991200,
    -0.011097551616033100,
    -0.023377317298097600,
    -0.014560284217671000,
    0.003582582016680210,
    0.014549180415936700,
    0.011162261151526300,
    -0.000735938093828361,
    -0.009741273892249770,
    -0.009231015015776210,
    -0.001674768165905220,
    0.005315470630395730,
    0.006257195418023980,
    0.001768705442799040,
    -0.003290070348925910,
    -0.004716082694953380,
    -0.002218475374737840,
    0.001264577750721040,
    0.002720057594105130,
    0.001516983112219500,
    -0.000681925402542081,
    -0.001854545301167810,
    -0.001346407587574020,
    -0.000026672117250873,
    0.000837678462105258,
    0.000715438143813428,
    0.000019591792777618,
    -0.000513457595923930,
    -0.000516949697820644,
    -0.000166720581170573,
    0.000143919934167101,
    0.000196248384784120,
    0.000057887247193660,
    -0.000080800014418548,
    -0.000109470418324297,
    -0.000054368878976467
};
