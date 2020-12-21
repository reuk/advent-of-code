#include <catch2/catch.hpp>

#include <charconv>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <variant>

namespace {
auto constexpr test_input = R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish)
)";

auto constexpr input =
    R"(gmgzk lpdz pqnvb kvbmftc bcnt xfqnss qpnvcj nct bzxl rfdspl jzzjz sncdd ntbvc csrhqtx ktmxcn sgxqfj hcql vfmmq xxjkk csmrb gjddl pzzghh vtfsc tnk rsdnp kbtrd pssbs mljgf gbthm mlkv vrcln jmttf gnthh mtbbmqx dxvsp skdj kmsdzs qjtlb zmdxkbl pllzxb hvtn tftzqv rrp ghk rgpl lrfhq grlj blpflv zmqm jzrks znhdgj fpfzjd zqtrtmt gjb lfcl bxlcs gxhf mlsgjf vspv clsfjdj fkxsxc bxkrd hkl llnhqgx cggjv fppqtg vvx gjtttz dkk szhpqll lzlq mdvm fkp rvcgr cjzfxzt mqngs rtdfn qmfg tqzgb rfbzdxpt dtxnkhj rfxr (contains peanuts, eggs, wheat)
mbdbdt skpqxj xmbr bcqqrr rdrfcd xrrlfcs vtfsc kznc bpqm csmrb dzkb vml xzdzl dtcr rfxr csrhqtx rqkxrrt bfhfqp jmfgh zmqm sncdd kmsdzs qvzd tftzqv gjddl jzzjz bkmfl vfgp gbthm bxkrd znbs vvx ntbvc nzjkbs gznnn gkzs vzxrgn jdhzzz hrf clsfjdj glcs fkxsxc xjgfn srbjjv tzxqf cggjv slqbc bzxl brtnxz fvpcbh kvbmftc bqblkh zqtrtmt ktmxcn qjtlb xncd vrcp jvhjxn dxvsp crqz lblqc vrcln fppqtg hbdcs hcql nptsm xfqnss mdvm qtbgl gsprc pllzxb (contains fish, sesame)
tqzgb gnthh jhrbjsr kshxlf mdvm bfhfqp rsdnp jhf nbkrmh vspv rmdlkn kmsdzs lcmvc sncdd lrfhq tlhj xfqnss jzzjz gbthm nkth fqzkg sgxqfj gxhf ntbvc xncd hcql llgjg hjc bhsvqks bhflr dxvsp bzxl jzrks fppqtg vkdj bkmfl mlkv pssbs gpjstjg jdhzzz znhdgj bxkrd vjxfx srbjjv sstkmq jktsl fkxsxc rfdspl tzxqf sq fkp lzlq dxvnl vml thkp qtbgl gjb dzkb tjvtpjq nq fqgccl nzjkbs bxlcs nptsm ghk rdrfcd brrb lfcl gjddl hgtzzp pqnvb vrcp rqkxrrt jlmvcn sqqg gkzs rvcgr (contains wheat)
nptsm bpqm gjtttz nq gpjstjg pllzxb skdj bkmfl qtbgl nkth gmgzk zqtrtmt ncdrm csmrb hkvglb dzkb jhf xpk bcnt clsfjdj vvx rrp csrhqtx rznvzm llgjg jmttf grlj znhdgj bxlcs pzzghh lfcl dbfx mftgt vfgp ffqpft bbxkm mlkv ktmxcn tjvtpjq pqnvb pzdsp brrb dbkz ghk hkl gsmg hnvcpljs tsxkcg nct bhflr tnk lcmvc bqblkh st hmgz vcpfq hbdcs zmdxkbl bxkrd dxvsp gjb znbs lcdd thkp fvpcbh brtnxz qmfg lbnmnf xfqnss sq svpth vspv bzxl kshxlf dtxnkhj rvcgr blpflv gjddl vjxfx (contains sesame, fish, shellfish)
bcqqrr gjb llnhqgx bhsvqks kvbmftc lblqc nzjkbs bfhfqp hkl gbthm mbdbdt kbtrd dtxnkhj sq qpnvcj vfmmq rgpl hffsbh zgj fvpcbh rrp gmgzk slqbc fmjpdnv pllzxb bkmfl lbnmnf gjddl hrf qmfg thkp fnjz nkth dbkz mlsgjf skpqxj pzdsp mljgf gjtttz nct jhf hcql dxvsp hbdcs svpth vspv tfqzgb xjgfn glcs zmqm sgxqfj tzxqf lcdd vtfsc jmfgh sqqg nvbhlj dzkb hkvh tqzgb vrcln blpflv vzxrgn jdhzzz gsprc ktmxcn xfqnss mqngs bbxkm kjd srbjjv lcmvc zmdxkbl mlkv lzlq fqzkg jzzjz dxvnl fppqtg gsmg (contains fish)
vfgp vrcln sstkmq jhf rfbzdxpt jzzjz xfqnss bpqm fpfzjd gmgzk fkxsxc ncdrm lpdz znbs fh vrcp skdj dxvsp gsprc nzjkbs ktmxcn hbdcs znhdgj hvtn hmgz xpk qmfg fqzkg ntbvc lcmvc rgpl vzxrgn gpjstjg qtxd hnvcpljs tzxqf pllzxb dxvnl bxkrd tjvtpjq ffnsz zmqm mlkv gjb qjtlb hgtzzp qtbgl nkth gjddl slqbc dzkb hrf (contains peanuts, wheat, sesame)
hffsbh vcpfq rfdspl crqz xrrlfcs fpfzjd tlhj qtbgl dzkb llgjg glcs mtbbmqx csrhqtx hnvcpljs bxkrd jdhzzz rfbzdxpt rvcgr nkth bkmfl jlmvcn bxlcs sstkmq dbfx fh kbtrd dtcr srqhhvfp xfqnss vfgp brrb txgdclk tzxqf pllzxb mftgt jz lfcl qqczc mlgch kmsdzs csmrb nptsm jmttf lpdz knfdh gjddl sncdd nq jzzjz hbdcs fmjpdnv hcql dxvsp grlj bcqqrr sgxqfj bzxl mlrmmt (contains peanuts)
lfcl pllzxb szhpqll dzkb bqblkh jzzjz nptsm xfqnss gsmg hcql bxkrd mlgch lrfhq rrp bbxkm pqnvb mlsgjf gsprc bktfm lblqc gpjstjg gznnn vspv cz hbdcs kznc gjtttz bcqqrr hnvcpljs rdrfcd lzlq hffsbh ghk jktsl znbs vzxrgn jz fvpcbh bhsvqks lbnmnf zqtrtmt lcmvc rqkxrrt dbkz kvbmftc dkk rgpl dxvsp fkp nq skdj dbfx vrcln bkmfl rtdfn (contains fish, wheat, eggs)
znbs crqz gnthh zmdxkbl mdvm pqnvb hgtzzp dzkb dtcr rdrfcd st gjddl thkp bfhfqp bpmpfs hrf jktsl vspv mqngs ljtb rtdfn brrb tftzqv jz nzjkbs ffnsz pllzxb kmzlpb hkmrt fpfzjd sncdd pssbs llgjg bxkrd ncdrm bkmfl rfxr bbxkm bhsvqks cggjv hnvcpljs qtxd jzzjz dxvsp (contains nuts, sesame)
bzxl srbjjv mbdbdt qmfg gznnn flfpkf pllzxb xncd ghk fqgccl fppqtg qtbgl dxvsp qjzzmx dxvnl jlmvcn bhsvqks sgxqfj fpfzjd tnk jdhzzz zqtrtmt rqkxrrt mlrmmt dkk jzzjz txgdclk csmrb kvbmftc rgpl vvx skpqxj grlj gmgzk dbkz ljtb hbdcs pssbs svpth lfcl vspv hvtn qqczc rfbzdxpt xfqnss rrp gjddl vml slqbc gnthh sstkmq bxkrd qvzd brrb crqz pzdsp rtdfn bhflr nct rdrfcd vcpfq hnvcpljs lcdd znbs bkmfl hmgz ncdrm nq kjd vtfsc lpdz hgtzzp sq hkmrt fkxsxc (contains peanuts, shellfish, wheat)
vzxrgn lfcl ghk vfgp xfqnss vspv bxkrd sq mtbbmqx sgxqfj hvtn dbfx srbjjv gdt gjb lrfhq pzdsp nzjkbs bhsvqks jzzjz fkxsxc xncd lbnmnf gnthh xpk clsfjdj bfhfqp qqczc dzkb jmttf znhdgj kmzlpb cz mljgf jhrbjsr dxvsp zgj gjddl bqblkh vkdj mlgch csmrb qtbgl zmqm sncdd skpqxj gbthm hffsbh lpdz sqqg gsprc kznc fpfzjd hmgz mqngs bkmfl gmgzk xmbr st pssbs vfmmq (contains dairy, nuts, wheat)
xxjkk fqzkg fppqtg zqtrtmt qpnvcj thkp tftzqv kjd mtbbmqx gdt cjzfxzt jlmvcn bxlcs hmgz rtdfn srbjjv rfbzdxpt vrcln jz mlsgjf gmgzk mlgch bzxl vspv znbs ffnsz dxvsp fkp fpfzjd dzkb dtxnkhj hkl xfqnss sncdd xzdzl dbkz fnjz gsprc rmdlkn rfdspl pllzxb jzzjz nct hvtn xrrlfcs csmrb tsxkcg glcs hcql sgxqfj mqngs mdvm slqbc ktmxcn knfdh zgj hkvh sstkmq bcqqrr gkzs rqkxrrt srqhhvfp bxkrd jvhjxn bcnt kznc vcpfq bbxkm hkmrt ffqpft rdrfcd bfhfqp pssbs bkmfl (contains fish, eggs)
gpjstjg rznvzm fkxsxc pllzxb mbdbdt mqngs dtxnkhj cz bxkrd skdj jhrbjsr xfqnss fqgccl gjddl csrhqtx srqhhvfp dxvsp pzzghh tftzqv hbdcs tlhj lrfhq rgpl vzxrgn brtnxz nzjkbs qtbgl zqtrtmt sncdd bhsvqks ntbvc vcpfq kmsdzs tzxqf blpflv sqqg llgjg znhdgj qqczc tsxkcg jmfgh fqzkg rfdspl jzzjz jktsl gjb hvtn rdrfcd jmttf hkvh vfmmq zmdxkbl vvx cjzfxzt gkzs mdvm vspv hkmrt rmdlkn hjc nbkrmh fkp dtcr (contains fish, eggs, peanuts)
tqzgb vspv zqtrtmt qmfg bbxkm rfdspl mdvm skpqxj nct grlj bhflr qvzd gsprc fqgccl lblqc gsmg bcqqrr xpk dxvsp nzjkbs znhdgj gjb hjc vtfsc hkl tzxqf zmdxkbl csmrb gkzs nptsm thkp glcs hbdcs qjzzmx mljgf hvtn ktmxcn zgj szhpqll gdt brtnxz st nq qpnvcj sgxqfj ljtb brrb xfqnss dzkb lpdz rfxr rtdfn xxjkk pllzxb tnk dtcr dtxnkhj mqngs vfmmq blpflv gxhf jz vml crqz hmgz vjxfx mtbbmqx hkmrt mlrmmt fkp lzlq vrcln fkxsxc tlhj slqbc fqzkg rvgfqs rqkxrrt vkdj knfdh fpfzjd fmjpdnv jmfgh jhf bxkrd jhrbjsr pssbs kjd rfbzdxpt srbjjv gjddl hffsbh rznvzm (contains fish, wheat)
llnhqgx bpqm tsxkcg jlmvcn dzkb lrfhq pllzxb rznvzm bbxkm mlrmmt dkk gbthm fmjpdnv kmzlpb bktfm dtcr gsprc bcqqrr lcmvc tftzqv xncd bkmfl gdt blpflv vzxrgn jzrks tfqzgb bcnt gjddl vkdj hmgz kshxlf xmbr vspv rgpl xjgfn xfqnss lfcl skpqxj jmfgh jzzjz qpnvcj bzxl cjzfxzt fkp gpjstjg dxvsp jvhjxn srqhhvfp rsdnp dtxnkhj zmqm xpk (contains dairy)
xrrlfcs hkvglb vtfsc vspv pllzxb mdvm lfcl ktmxcn dkk lrfhq xfqnss qmfg pzzghh dxvnl fkxsxc rvcgr tqzgb ghk tnk bbxkm ljtb mlrmmt vfgp nzjkbs sq mljgf qjtlb mtbbmqx kznc jzrks kshxlf dxvsp sqqg sgxqfj glcs kmsdzs dtxnkhj cjzfxzt knfdh qjzzmx gznnn ncdrm nvbhlj gjddl fmjpdnv lblqc xjgfn lcmvc rsdnp mftgt fppqtg hbdcs bhflr zqtrtmt qqczc pssbs lpdz jhf gxhf gkzs fvxx lzlq sncdd llgjg znbs kvbmftc qvzd nq bkmfl grlj mqngs skpqxj gnthh bpqm gsmg hgtzzp tftzqv srbjjv ntbvc gdt lcdd tjvtpjq tzxqf brrb tsxkcg gmgzk dzkb fvpcbh hmgz vcpfq jvhjxn jzzjz bktfm bxlcs zmdxkbl gjtttz (contains sesame, shellfish)
hmgz sq mdvm bcnt xfqnss jdhzzz fqzkg ncdrm lpdz kznc xpk csmrb gjddl gbthm flfpkf hrf gdt qjzzmx rfbzdxpt pzzghh zgj znhdgj bqblkh rfdspl xxjkk jhrbjsr csrhqtx jzrks fqgccl grlj tsxkcg jzzjz gznnn vspv dtcr mljgf bpmpfs tfqzgb tlhj gjtttz llgjg clsfjdj rznvzm fh xncd dzkb srqhhvfp bfhfqp jz bxkrd dxvsp cjzfxzt (contains eggs, peanuts, shellfish)
sqqg jzzjz tzxqf lfcl zqtrtmt qmfg lbnmnf bhsvqks bkmfl ffqpft vspv ghk xfqnss nq jhf pllzxb hkvglb qtxd vrcln znhdgj hkl thkp rtdfn bbxkm nkth qpnvcj qtbgl mljgf cjzfxzt clsfjdj gdt gpjstjg gznnn dxvsp hbdcs tfqzgb hmgz mlsgjf mlgch gjtttz dzkb kznc rmdlkn vml qvzd xncd fkxsxc mqngs gbthm lpdz hrf qjzzmx gjddl fqgccl ljtb hcql srqhhvfp fmjpdnv llnhqgx (contains dairy, shellfish, peanuts)
bkmfl jhrbjsr fqzkg jktsl gsprc rtdfn bpmpfs skpqxj dkk tftzqv gjddl mljgf nptsm kjd mbdbdt fvpcbh xzdzl pllzxb mqngs qqczc vfgp rgpl fmjpdnv brtnxz vml jvhjxn bzxl lblqc dxvsp bxkrd sncdd fvxx dzkb vzxrgn blpflv pssbs hrf ncdrm hnvcpljs gznnn bqblkh tlhj kbtrd jzzjz nvbhlj xfqnss fkxsxc (contains dairy, fish, sesame)
hcql nzjkbs st rsdnp gkzs gxhf hkvglb mqngs blpflv qjtlb rqkxrrt mdvm nkth vzxrgn bhsvqks qmfg dbkz fkp csmrb mlkv xjgfn tlhj vtfsc cggjv bpqm xfqnss dbfx fvxx tfqzgb gjddl jz skpqxj fpfzjd bxkrd kmzlpb srqhhvfp lrfhq dxvsp sgxqfj kshxlf xpk dxvnl qjzzmx jzzjz slqbc cjzfxzt hjc flfpkf srbjjv dzkb csrhqtx gdt vspv sq kbtrd qtxd rznvzm rgpl dtcr fh bbxkm pssbs ljtb vfgp gnthh (contains wheat, peanuts, dairy)
pllzxb dzkb vspv bxkrd pssbs xjgfn rfdspl vvx hjc lcmvc dxvsp glcs vfgp lbnmnf kmzlpb xncd hvtn szhpqll sqqg bfhfqp znhdgj lcdd vkdj skpqxj kshxlf nq qpnvcj slqbc ffnsz dbfx zgj gmgzk jdhzzz thkp nvbhlj llgjg gjtttz rsdnp gjddl jhrbjsr gnthh bzxl srqhhvfp qtxd dkk xpk nct mlgch hbdcs pqnvb srbjjv rrp rgpl sq vzxrgn crqz xfqnss ljtb hnvcpljs fppqtg pzzghh hmgz nptsm (contains nuts, shellfish)
sq hbdcs qtxd mtbbmqx gsmg fkxsxc mljgf jzzjz clsfjdj nct gmgzk bkmfl hkmrt gxhf fvpcbh dxvsp bxlcs srbjjv znhdgj vrcp bhsvqks hffsbh pllzxb xfqnss dzkb thkp gpjstjg sstkmq tqzgb tjvtpjq bktfm qpnvcj gjddl rfdspl zqtrtmt kjd gkzs zgj mlkv bxkrd xzdzl szhpqll rfbzdxpt (contains wheat, peanuts, eggs)
kmsdzs kvbmftc clsfjdj bxkrd xfqnss fmjpdnv bkmfl nzjkbs kbtrd sgxqfj jhf rdrfcd gpjstjg jmfgh vfmmq hnvcpljs dkk txgdclk pqnvb blpflv nct pllzxb qjtlb hgtzzp qvzd vrcln rfdspl pzzghh mbdbdt mlrmmt vvx zqtrtmt fvxx vzxrgn rgpl gkzs bktfm gdt jz hjc xpk tjvtpjq gxhf ffnsz fh jlmvcn lcmvc jzzjz nbkrmh fkp lpdz lzlq jdhzzz hkmrt rznvzm sstkmq cz pssbs qpnvcj qqczc hcql sncdd mlgch srqhhvfp nvbhlj skdj llnhqgx szhpqll gjddl tnk vrcp fvpcbh crqz grlj dzkb nq tsxkcg rmdlkn rfxr gjb jhrbjsr gsmg ktmxcn dxvsp rqkxrrt (contains eggs)
fmjpdnv gnthh vzxrgn hjc vml llgjg ffnsz bkmfl lrfhq jlmvcn dkk bktfm fqgccl knfdh skpqxj xzdzl cggjv rvcgr jzzjz kbtrd gsmg rqkxrrt hkvglb qmfg fvxx pzzghh jhrbjsr hmgz sq xfqnss fkxsxc dtcr hnvcpljs gmgzk llnhqgx rtdfn mftgt gjddl bhflr pllzxb dbkz cjzfxzt gznnn dxvnl ktmxcn kmsdzs dbfx szhpqll gbthm dxvsp hgtzzp gjtttz thkp kjd lblqc bxlcs bxkrd gjb dzkb mlgch vrcln vtfsc xpk ghk rgpl cz xrrlfcs mtbbmqx mdvm ntbvc rrp pqnvb jzrks fh lcmvc jhf hbdcs slqbc hrf kshxlf (contains wheat, eggs)
bhflr st rsdnp xfqnss zmdxkbl znhdgj szhpqll ljtb dtcr vspv llnhqgx pllzxb nptsm rfbzdxpt tfqzgb thkp kbtrd fqzkg srbjjv lzlq rvgfqs jhrbjsr sqqg qtbgl nq kjd rznvzm bxlcs lfcl jzzjz fkxsxc zgj mtbbmqx gpjstjg xjgfn xzdzl dxvsp qjtlb gdt lpdz hkvh rfdspl bbxkm vzxrgn hrf bxkrd jz ghk xxjkk lcdd skdj sstkmq mlrmmt txgdclk skpqxj dzkb fh (contains peanuts)
xmbr xrrlfcs jzzjz kjd vjxfx xfqnss bpqm csmrb vspv bbxkm kshxlf hcql blpflv fh slqbc sqqg tsxkcg grlj csrhqtx zgj hnvcpljs cz rmdlkn dkk nptsm dxvsp thkp pllzxb srbjjv xpk kbtrd brtnxz vvx gjddl jlmvcn rvcgr rznvzm nzjkbs gpjstjg tqzgb fpfzjd szhpqll bxkrd ffqpft tzxqf dtxnkhj crqz hffsbh ljtb gjtttz vrcp vml dbkz srqhhvfp tjvtpjq (contains nuts, peanuts)
hnvcpljs jzzjz qjtlb brrb xfqnss hbdcs nptsm vtfsc fvpcbh vvx jmttf xjgfn jvhjxn rgpl mlkv csmrb dtxnkhj dxvsp brtnxz vrcln svpth bzxl pllzxb bhflr hkl srqhhvfp lzlq hkmrt jzrks hjc grlj clsfjdj lcdd bpqm gmgzk gbthm mftgt gsprc sqqg vml lpdz sq dzkb bqblkh hrf vfmmq rfxr tsxkcg gkzs bxkrd vspv kmzlpb blpflv ffnsz jdhzzz knfdh bkmfl vrcp (contains wheat, dairy)
hvtn lblqc fqgccl qqczc dbfx mtbbmqx hjc xfqnss jktsl csrhqtx dzkb vcpfq jzzjz lrfhq pqnvb svpth xmbr bfhfqp sgxqfj vml nkth gxhf mlrmmt bxkrd kmzlpb zgj hgtzzp ffqpft pzzghh ncdrm thkp jhrbjsr ghk jdhzzz rmdlkn fppqtg rdrfcd cggjv gjb gnthh hffsbh glcs gznnn rgpl nct ljtb vspv mbdbdt brrb txgdclk gdt dxvnl nbkrmh nq jlmvcn jz qmfg tfqzgb lcmvc csmrb sncdd xrrlfcs dxvsp mljgf qtxd sqqg kvbmftc clsfjdj pllzxb znhdgj pssbs vvx fvpcbh srqhhvfp (contains eggs, shellfish)
rgpl jktsl hffsbh txgdclk pllzxb nbkrmh gjtttz hkl dkk nct jzzjz rfdspl xfqnss fqzkg bkmfl fh qjtlb bcqqrr lpdz slqbc svpth jzrks hkvh qmfg clsfjdj rfxr kmsdzs bktfm bxlcs lrfhq ffqpft tnk sncdd mdvm dzkb vtfsc hkvglb gnthh knfdh dxvsp hjc bxkrd jmfgh mlsgjf skdj ljtb hvtn thkp fmjpdnv bbxkm cjzfxzt hgtzzp rvcgr crqz rznvzm ghk hkmrt vspv dtxnkhj fkp dbfx grlj rtdfn vml gbthm qtbgl (contains shellfish, wheat)
qpnvcj bqblkh xrrlfcs dbkz gpjstjg glcs gjtttz dxvsp pllzxb csmrb mljgf flfpkf dzkb vvx dxvnl rvcgr hbdcs jzzjz lblqc tftzqv gznnn kmsdzs blpflv gnthh bhflr csrhqtx vspv sqqg fkp skpqxj bkmfl mlrmmt nptsm pssbs szhpqll nct tqzgb bcnt rmdlkn xmbr skdj hrf gdt vrcp qqczc gkzs zqtrtmt lfcl bxkrd clsfjdj pzdsp svpth bfhfqp hnvcpljs dtcr bpqm hkmrt vjxfx xfqnss ffqpft qvzd mlkv tzxqf vzxrgn bpmpfs st rdrfcd jhf fmjpdnv rqkxrrt tlhj qmfg rfdspl (contains fish)
hkvglb gjb mtbbmqx crqz skdj kbtrd rgpl hkl qjzzmx bqblkh thkp hcql zgj fqzkg pllzxb jz fvxx lbnmnf fvpcbh fnjz gkzs dxvsp ffnsz gxhf dkk hrf gpjstjg sgxqfj hbdcs sstkmq qmfg jktsl skpqxj rznvzm gjddl rfdspl st mftgt xfqnss jmfgh gdt rfbzdxpt ncdrm hnvcpljs rtdfn jzzjz szhpqll dzkb fppqtg kmsdzs jzrks jlmvcn srbjjv bxkrd qpnvcj sncdd lpdz xrrlfcs xpk (contains nuts, fish, sesame)
hkvh rvcgr xjgfn clsfjdj kjd crqz jvhjxn jzzjz dbfx tzxqf hgtzzp zmqm bxkrd jhrbjsr kznc tqzgb pqnvb csmrb brtnxz dxvsp sncdd lcdd hcql gmgzk vfgp gznnn fqgccl bcnt xpk zqtrtmt mtbbmqx pssbs xfqnss ghk cjzfxzt tjvtpjq dbkz xncd dzkb jzrks qqczc ffnsz glcs gjb rznvzm bzxl rdrfcd gjddl hnvcpljs vspv rvgfqs srbjjv jktsl lzlq fvpcbh mlkv nct ktmxcn ntbvc (contains sesame)
nbkrmh pzdsp bhsvqks dxvsp brtnxz llnhqgx hrf rmdlkn hmgz gsmg gsprc gjddl fvpcbh pssbs thkp qtxd sgxqfj lfcl rfxr qjtlb llgjg jzzjz gjtttz tjvtpjq ghk kznc hkmrt bkmfl nzjkbs dzkb bxkrd vspv lpdz kvbmftc zgj kjd qpnvcj fqgccl mljgf bhflr ljtb xjgfn srqhhvfp rvgfqs skdj grlj pllzxb (contains nuts)
srbjjv bkmfl rvgfqs nvbhlj dtxnkhj fh kmzlpb szhpqll hrf blpflv sqqg xfqnss vzxrgn mftgt lbnmnf zmqm fkp gjb xzdzl flfpkf mqngs ghk hkvglb bpmpfs mlsgjf dxvsp mlgch thkp brrb st llnhqgx rfbzdxpt fvpcbh gsprc bpqm fvxx qtxd brtnxz bxlcs csrhqtx vjxfx gmgzk bxkrd rdrfcd hnvcpljs sgxqfj kshxlf qtbgl dtcr pllzxb mljgf nq gjtttz lzlq bcqqrr mlkv bqblkh bcnt lrfhq qmfg kznc zqtrtmt glcs xxjkk qjtlb ktmxcn vcpfq vspv nptsm crqz rfxr jdhzzz rvcgr hkmrt rznvzm dzkb rfdspl zgj gjddl mbdbdt (contains peanuts, sesame)
gsprc gdt rfbzdxpt mqngs dxvsp tqzgb mftgt hrf mlgch dbkz lzlq ntbvc cggjv ktmxcn pllzxb xncd zmqm fqzkg brrb vspv blpflv bcnt xzdzl tlhj dzkb bpqm rgpl cjzfxzt nct ljtb sstkmq gjddl ffnsz xpk rfxr vtfsc nptsm fppqtg qjtlb brtnxz xmbr jmfgh fvxx xfqnss kvbmftc rznvzm znhdgj gjb mlsgjf jvhjxn bfhfqp cz kshxlf srbjjv fmjpdnv hgtzzp jzzjz gkzs csmrb vjxfx gnthh rtdfn mlrmmt mbdbdt fkp lbnmnf (contains dairy, wheat)
vfgp mqngs thkp bzxl kznc dxvnl bcnt st mlrmmt dzkb tftzqv mdvm kbtrd pzzghh pllzxb jzzjz bhsvqks jhf lbnmnf nptsm hkvh bcqqrr zqtrtmt vrcp rfbzdxpt xrrlfcs lzlq ktmxcn pzdsp tfqzgb llnhqgx crqz xzdzl bhflr vspv tsxkcg nbkrmh bxkrd llgjg srbjjv pssbs fnjz mlgch dxvsp mbdbdt qtbgl nct sstkmq bpmpfs flfpkf cz xfqnss skdj bbxkm fppqtg rfxr jmttf mlsgjf gmgzk rrp hgtzzp knfdh sq bqblkh clsfjdj fpfzjd sqqg gznnn rqkxrrt slqbc jzrks nzjkbs kvbmftc csmrb nq gkzs brtnxz szhpqll hrf rsdnp hcql grlj kjd (contains eggs)
csrhqtx kmzlpb mftgt ffnsz kznc srbjjv sgxqfj gkzs qjtlb lrfhq dbfx vjxfx gpjstjg mlgch jhrbjsr nvbhlj tzxqf jlmvcn hjc xrrlfcs svpth bpqm knfdh vspv qtbgl jmfgh mlkv bhflr sstkmq szhpqll fmjpdnv fqgccl gbthm nq tsxkcg fqzkg bfhfqp jz gznnn grlj hvtn pllzxb qpnvcj jzzjz dzkb cjzfxzt vrcp brrb vvx fvpcbh dxvsp xncd llgjg hffsbh bcqqrr jdhzzz rfxr mljgf hcql blpflv vkdj xxjkk tjvtpjq bxkrd mdvm rsdnp xfqnss dbkz jvhjxn srqhhvfp fkp zmdxkbl hrf lbnmnf znhdgj pzzghh tftzqv (contains dairy, wheat)
gnthh ncdrm hmgz lfcl rtdfn rznvzm bhflr xpk qvzd fqgccl vjxfx vrcp crqz gxhf rvcgr xfqnss skdj mlkv jzzjz xjgfn flfpkf bbxkm jhrbjsr nct qtbgl vkdj vspv gjddl hkl fnjz jzrks dxvsp glcs hrf dtcr pllzxb zgj bxkrd clsfjdj bktfm thkp vvx (contains dairy, fish, wheat)
mbdbdt bxkrd hnvcpljs kvbmftc brrb zgj jzzjz csrhqtx hkvh txgdclk pzzghh vml xxjkk gdt gjddl dxvsp lzlq mlsgjf sncdd rrp hcql nptsm lcdd mftgt xmbr xfqnss jhrbjsr brtnxz bcnt kmzlpb flfpkf kbtrd rvcgr nq gznnn hbdcs sq hmgz rmdlkn cz gnthh fpfzjd hrf hffsbh jhf glcs bhflr gjb fppqtg hkvglb dtxnkhj skpqxj bpqm vcpfq dzkb kmsdzs bzxl tftzqv vjxfx vfmmq kznc sqqg jz srqhhvfp xzdzl st dbfx gmgzk bpmpfs mdvm cggjv tsxkcg tfqzgb lpdz mqngs xrrlfcs vfgp fvxx fqgccl csmrb pzdsp bhsvqks zmdxkbl tqzgb jmttf vspv gsprc gkzs lfcl (contains shellfish, wheat, peanuts)
bfhfqp qjtlb vspv jzrks ffqpft hkvh tjvtpjq tnk bzxl zgj zmdxkbl gsmg pllzxb gjb fvxx hffsbh vkdj mljgf xzdzl hmgz kmsdzs ntbvc thkp rmdlkn jhrbjsr sstkmq gjddl gsprc gmgzk kvbmftc hjc lpdz hkl jz bbxkm bxkrd mftgt gdt rdrfcd kbtrd mlrmmt mtbbmqx qtbgl jdhzzz jzzjz rrp rtdfn hkvglb xfqnss lrfhq dzkb vrcp nct fkxsxc ghk hgtzzp (contains peanuts)
dzkb grlj xrrlfcs rqkxrrt gjtttz vspv szhpqll glcs sncdd thkp gnthh gjddl bkmfl csrhqtx pssbs flfpkf hkvh vcpfq dxvnl lpdz ktmxcn fqgccl gsmg gkzs vfgp vml fkp cjzfxzt nct fvxx lzlq qmfg nkth bktfm pllzxb pqnvb lcdd ghk jvhjxn jmttf tftzqv jz srqhhvfp hkvglb knfdh jzzjz hmgz fmjpdnv nvbhlj clsfjdj bcnt rvcgr xfqnss kshxlf hgtzzp brtnxz gxhf jlmvcn gsprc hvtn bxkrd mqngs (contains peanuts, nuts, sesame)
)";

struct Line {
  std::set<std::string> ingredients;
  std::set<std::string> allergens;
};

std::vector<std::string> split(std::string text, std::string_view delimit) {
  std::vector<std::string> result;

  for (size_t pos = 0; pos < text.size(); pos += delimit.size()) {
    auto const last_pos = std::exchange(pos, text.find(delimit, pos));
    result.emplace_back(text.substr(last_pos, pos - last_pos));

    if (pos == std::string::npos)
      break;
  }

  result.erase(std::remove_if(result.begin(), result.end(),
                              [](auto const &t) { return t.empty(); }),
               result.cend());

  return result;
}

std::vector<std::string> multisplit(std::string text,
                                    std::span<std::string const> delimiters) {
  std::vector<std::string> input{text};
  std::vector<std::string> output;

  for (auto const &delimiter : delimiters) {
    output.clear();

    for (auto const &word : input) {
      auto const out = split(word, delimiter);
      output.insert(output.cend(), out.cbegin(), out.cend());
    }

    input = output;
  }

  return output;
}

std::set<std::string> to_set(std::span<std::string const> s) {
  return {s.cbegin(), s.cend()};
}

std::istream &operator>>(std::istream &is, Line &line) {
  line.ingredients.clear();
  line.allergens.clear();

  std::string text;
  std::getline(is, text);

  if (std::smatch m; std::regex_match(
          text, m, std::regex{R"(([a-z ]+) \(contains ([a-z, ]+)\))"})) {
    line.ingredients = to_set(split(m[1], " "));
    line.allergens = to_set(split(m[2], ", "));
  }

  return is;
}

template <typename Key, typename Value, typename Fn>
void erase_if(std::map<Key, Value> &m, Fn &&predicate) {
  for (auto it = m.begin(); it != m.end();) {
    if (predicate(*it))
      it = m.erase(it);
    else
      ++it;
  }
}
} // namespace

TEST_CASE("day21") {
  auto const lines = [&] {
    std::istringstream is{input};
    return std::vector(std::istream_iterator<Line>{is},
                       std::istream_iterator<Line>{});
  }();

  auto const possibilities = [&] {
    std::map<std::string, std::set<std::string>> possibilities;

    for (auto const &line : lines) {
      for (auto const &allergen : line.allergens) {
        if (auto const it = possibilities.find(allergen);
            it != possibilities.cend()) {
          std::set<std::string> new_set;
          std::set_intersection(
              it->second.cbegin(), it->second.cend(), line.ingredients.cbegin(),
              line.ingredients.cend(), std::inserter(new_set, new_set.end()));
          it->second = std::move(new_set);
        } else {
          possibilities.emplace(allergen, line.ingredients);
        }
      }
    }

    return possibilities;
  }();

  auto const all_ingredients = [&] {
    std::set<std::string> all_ingredients;

    for (auto const &line : lines)
      all_ingredients.insert(line.ingredients.cbegin(),
                             line.ingredients.cend());

    return all_ingredients;
  }();

  auto const filtered_ingredients = [&] {
    auto filtered_ingredients = all_ingredients;

    for (auto const &possibility : possibilities)
      for (auto const &ingredient : possibility.second)
        filtered_ingredients.erase(ingredient);

    return filtered_ingredients;
  }();

  auto const a = [&] {
    auto count = 0;

    for (auto const &line : lines)
      for (auto const &ingredient : line.ingredients)
        if (filtered_ingredients.contains(ingredient))
          count += 1;

    return count;
  }();

  auto const b = [&] {
    std::map<std::string, std::string> identified;
    auto filtered_possibilities = possibilities;

    while (!filtered_possibilities.empty()) {
      auto const it = std::find_if(
          filtered_possibilities.cbegin(), filtered_possibilities.cend(),
          [](auto const &pair) { return pair.second.size() == 1; });

      if (it == filtered_possibilities.cend())
        break;

      auto const &[key, value] = *it;
      auto const allergen = *value.begin();
      identified[key] = allergen;

      for (auto &[inner_key, inner_value] : filtered_possibilities)
        inner_value.erase(allergen);

      erase_if(filtered_possibilities,
               [](auto const &p) { return p.second.empty(); });
    }

    std::string result;

    for (auto const &[ingredient, allergen] : identified)
      result += (allergen + ",");

    result.pop_back();
    return result;
  }();
}
