// Low-pass filter specification for up and down sampling

// Low-pass FIR - Remez Exchange Algorithm
// Fs:  48 kHz
// Fc:  7.2 kHz
// TBW: 800 Hz
// PBR: 1 dB
// SBA: -96 dB

#define LPF_FILTER_LENGTH 196

const SLData_t  LPFCoefficientArray[] = {
  -7.64091560385721658543e-05,
  1.74864674739995753800e-05,
  2.93437349592729239053e-04,
  7.75175004929250818612e-04,
  1.27966114275498000878e-03,
  1.50284163050766407695e-03,
  1.21490679666693725258e-03,
  4.69590240493852786700e-04,
  -3.56272092363455893278e-04,
  -7.76730422579843945638e-04,
  -5.46223417074843172608e-04,
  1.22405521198733224526e-04,
  6.80067693839797701889e-04,
  6.55241186008759593506e-04,
  4.74753959431221494829e-05,
  -6.39819611665587775762e-04,
  -7.98162112892563616745e-04,
  -2.42530125367538314934e-04,
  5.85209516332453952292e-04,
  9.55458015523668029861e-04,
  4.84699602201667278153e-04,
  -4.79683200605295892091e-04,
  -1.10177315524257065336e-03,
  -7.73267029611890421847e-04,
  3.02597385710649878325e-04,
  1.21199458088842274180e-03,
  1.09815158660757209753e-03,
  -4.11592145414860070309e-05,
  -1.26087884185783235917e-03,
  -1.44257129131031736242e-03,
  -3.11064091783471124812e-04,
  1.22278636571539894851e-03,
  1.78242214996127503780e-03,
  7.52336935905758858595e-04,
  -1.07433721197719911009e-03,
  -2.08797932767716253738e-03,
  -1.27153365888234824201e-03,
  7.98463430373699042247e-04,
  2.32880884412642189321e-03,
  1.85184640032972757523e-03,
  -3.83191179378589846793e-04,
  -2.47554048556657396024e-03,
  -2.47587043089799851286e-03,
  -1.87740844932219393300e-04,
  2.48489353637661170016e-03,
  3.10464216853539214450e-03,
  9.09072448601637851488e-04,
  -2.32766829764756606747e-03,
  -3.70456060388590479454e-03,
  -1.77794035542913272940e-03,
  1.96775717797093799982e-03,
  4.22867420278036018622e-03,
  2.77745831017742368652e-03,
  -1.37557442088607952829e-03,
  -4.62615050750458077500e-03,
  -3.88188210278174808399e-03,
  5.25998813673250699072e-04,
  4.84121307158522778957e-03,
  5.05512655542176506801e-03,
  5.99645498491154600382e-04,
  -4.81496289054464225721e-03,
  -6.25197496693308610305e-03,
  -2.01441533611591621841e-03,
  4.48454842390521787876e-03,
  7.41748607760484186813e-03,
  3.72600698713892097488e-03,
  -3.78203808200139367582e-03,
  -8.48598455482143902706e-03,
  -5.73708562148732021529e-03,
  2.63297175469535960080e-03,
  9.38090370891394784159e-03,
  8.04870420530725758090e-03,
  -9.51012428316165298742e-04,
  -1.00135106719253380581e-02,
  -1.06682098860082594932e-02,
  -1.37611696729027189676e-03,
  1.02752961465639343352e-02,
  1.36203870596611330973e-02,
  4.51264436574375267519e-03,
  -1.00220695575975026620e-02,
  -1.69683811127028377763e-02,
  -8.72686070600290839105e-03,
  9.04368216360342154492e-03,
  2.08688100804655905784e-02,
  1.45264599483773265698e-02,
  -6.97225498110578233157e-03,
  -2.56947962078856242951e-02,
  -2.30108188127670608425e-02,
  3.02705555864547001116e-03,
  3.24277435650220033359e-02,
  3.70782775345925991184e-02,
  4.95920050342146465083e-03,
  -4.43197940615617444471e-02,
  -6.74580667037072978509e-02,
  -2.65303679815297212274e-02,
  8.05043294438556322756e-02,
  2.11103999541017661734e-01,
  3.00269611201135766532e-01,
  3.00269611201135766532e-01,
  2.11103999541017661734e-01,
  8.05043294438556322756e-02,
  -2.65303679815297212274e-02,
  -6.74580667037072978509e-02,
  -4.43197940615617444471e-02,
  4.95920050342146465083e-03,
  3.70782775345925991184e-02,
  3.24277435650220033359e-02,
  3.02705555864547001116e-03,
  -2.30108188127670608425e-02,
  -2.56947962078856242951e-02,
  -6.97225498110578233157e-03,
  1.45264599483773265698e-02,
  2.08688100804655905784e-02,
  9.04368216360342154492e-03,
  -8.72686070600290839105e-03,
  -1.69683811127028377763e-02,
  -1.00220695575975026620e-02,
  4.51264436574375267519e-03,
  1.36203870596611330973e-02,
  1.02752961465639343352e-02,
  -1.37611696729027189676e-03,
  -1.06682098860082594932e-02,
  -1.00135106719253380581e-02,
  -9.51012428316165298742e-04,
  8.04870420530725758090e-03,
  9.38090370891394784159e-03,
  2.63297175469535960080e-03,
  -5.73708562148732021529e-03,
  -8.48598455482143902706e-03,
  -3.78203808200139367582e-03,
  3.72600698713892097488e-03,
  7.41748607760484186813e-03,
  4.48454842390521787876e-03,
  -2.01441533611591621841e-03,
  -6.25197496693308610305e-03,
  -4.81496289054464225721e-03,
  5.99645498491154600382e-04,
  5.05512655542176506801e-03,
  4.84121307158522778957e-03,
  5.25998813673250699072e-04,
  -3.88188210278174808399e-03,
  -4.62615050750458077500e-03,
  -1.37557442088607952829e-03,
  2.77745831017742368652e-03,
  4.22867420278036018622e-03,
  1.96775717797093799982e-03,
  -1.77794035542913272940e-03,
  -3.70456060388590479454e-03,
  -2.32766829764756606747e-03,
  9.09072448601637851488e-04,
  3.10464216853539214450e-03,
  2.48489353637661170016e-03,
  -1.87740844932219393300e-04,
  -2.47587043089799851286e-03,
  -2.47554048556657396024e-03,
  -3.83191179378589846793e-04,
  1.85184640032972757523e-03,
  2.32880884412642189321e-03,
  7.98463430373699042247e-04,
  -1.27153365888234824201e-03,
  -2.08797932767716253738e-03,
  -1.07433721197719911009e-03,
  7.52336935905758858595e-04,
  1.78242214996127503780e-03,
  1.22278636571539894851e-03,
  -3.11064091783471124812e-04,
  -1.44257129131031736242e-03,
  -1.26087884185783235917e-03,
  -4.11592145414860070309e-05,
  1.09815158660757209753e-03,
  1.21199458088842274180e-03,
  3.02597385710649878325e-04,
  -7.73267029611890421847e-04,
  -1.10177315524257065336e-03,
  -4.79683200605295892091e-04,
  4.84699602201667278153e-04,
  9.55458015523668029861e-04,
  5.85209516332453952292e-04,
  -2.42530125367538314934e-04,
  -7.98162112892563616745e-04,
  -6.39819611665587775762e-04,
  4.74753959431221494829e-05,
  6.55241186008759593506e-04,
  6.80067693839797701889e-04,
  1.22405521198733224526e-04,
  -5.46223417074843172608e-04,
  -7.76730422579843945638e-04,
  -3.56272092363455893278e-04,
  4.69590240493852786700e-04,
  1.21490679666693725258e-03,
  1.50284163050766407695e-03,
  1.27966114275498000878e-03,
  7.75175004929250818612e-04,
  2.93437349592729239053e-04,
  1.74864674739995753800e-05,
  -7.64091560385721658543e-05
};