#include "Buzzer.h"

namespace DT5EDU {

std::unordered_map<Tune,int> Tune2Freq = {
    // 3rd
    {Tune::C3, 262}, {Tune::C3s,277},
    {Tune::D3, 294}, {Tune::D3s,311},
    {Tune::E3, 330}, {Tune::F3, 349},
    {Tune::F3s,370}, {Tune::G3, 392},
    {Tune::G3s,415}, {Tune::A3, 440},
    {Tune::A3s,466}, {Tune::B3, 494},
    // 4th
    {Tune::C4, 523}, {Tune::C4s,554},
    {Tune::D4, 587}, {Tune::D4s,622},
    {Tune::E4, 659}, {Tune::F4, 698},
    {Tune::F4s,740}, {Tune::G4, 784},
    {Tune::G4s,831}, {Tune::A4, 880},
    {Tune::A4s,932}, {Tune::B4, 988},
    // 5th
    {Tune::C5, 1047},{Tune::C5s,1109},
    {Tune::D5, 1175},{Tune::D5s,1245},
    {Tune::E5, 1319},{Tune::F5, 1397},
    {Tune::F5s,1480},{Tune::G5, 1568},
    {Tune::G5s,1661},{Tune::A5, 1760},
    {Tune::A5s,1865},{Tune::B5, 1976}
};

buzzer & buzzer::SetTune(Tune tune_input) {
    if (tune_input == Tune::Pause) {
        IO.write(0.f);
    }
    else if (tune_input != Tune::Sustain) {
        IO.period(1.f/(Tune2Freq[tune_input]));
        IO.write(0.5f);
    }
    return *this;
}

buzzer & buzzer::PlayMusic(const vector<Tune> &Music, float NotePerMin) {
    auto delay_time = (int32_t)(60000/NotePerMin);
    for (auto &note: Music) {
        SetTune(note);
        ThisThread::sleep_for(delay_time);
    }
    SetTune();
    return *this;
};

buzzer & buzzer::PlayMusic(const vector<pair<Tune,float>> &Music, float NotePerMin) {
    auto delay_time = 60000/NotePerMin;
    for (auto &[note,beat]: Music) {
        SetTune(note);
        ThisThread::sleep_for(uint32_t(beat * delay_time));
    }
    SetTune();
    return *this;
};

buzzer & buzzer::PlayMusic(const vector<pair<Tune,float>> &Music, float NotePerMin, Ticker &tkr) {
    auto delay_time = 60000/NotePerMin;
    auto b = Music.begin();
    tkr.attach([&tkr,&delay_time] (void) {
        static int i;

    },delay_time * Music[0].second);
    return *this;
}

buzzer & buzzer::DemoMusic_1(void) {
    std::vector<Tune> music = {
        Tune::C4,Tune::C4,Tune::G4,Tune::G4,Tune::A4,Tune::A4,Tune::G4,Tune::Pause,
        Tune::F4,Tune::F4,Tune::E4,Tune::E4,Tune::D4,Tune::D4,Tune::C4,Tune::Pause,
        Tune::G4,Tune::G4,Tune::F4,Tune::F4,Tune::E4,Tune::E4,Tune::D4,Tune::Pause,
        Tune::G4,Tune::G4,Tune::F4,Tune::F4,Tune::E4,Tune::E4,Tune::D4,Tune::Pause,
        Tune::C4,Tune::C4,Tune::G4,Tune::G4,Tune::A4,Tune::A4,Tune::G4,Tune::Pause,
        Tune::F4,Tune::F4,Tune::E4,Tune::E4,Tune::D4,Tune::D4,Tune::C4,Tune::Sustain
    };
    PlayMusic(music, 120);
    return *this;
}

buzzer & buzzer::DemoMusic_2(void) {
    // 《春日影》
    // 使用pair<Tune,float>存储音符，前者存储音高，后者存储音的拍数
    std::vector<pair<Tune,float>> music = {
        {Tune::E4,2},  {Tune::D4,1},  {Tune::C4,2},  {Tune::D4,1},
        {Tune::E4,1.5},{Tune::F4,0.5},{Tune::E4,1},  {Tune::D4,3},
        {Tune::E4,2},  {Tune::D4,1},  {Tune::C4,2},  {Tune::D4,1},
        {Tune::E4,1.5},{Tune::F4,0.5},{Tune::E4,1},  {Tune::D4,3},
        {Tune::E4,2},  {Tune::D4,1},  {Tune::C4,2},  {Tune::D4,1},
        {Tune::E4,1.5},{Tune::F4,0.5},{Tune::E4,1},  {Tune::D4,3},
        {Tune::E4,2},  {Tune::D4,1},  {Tune::C4,2},  {Tune::D4,1},
        {Tune::E4,1.5},{Tune::F4,0.5},{Tune::E4,1},  {Tune::D4,2},  {Tune::G3,0.5},{Tune::G3,0.5},
        {Tune::E4,1},  {Tune::E4,1},  {Tune::D4,1},  {Tune::F4,1},  {Tune::E4,1},  {Tune::D4,1},
        {Tune::D4,1},  {Tune::D4,1},  {Tune::C4,0.5},{Tune::C4,0.5},{Tune::F4,1},  {Tune::E4,1},  {Tune::D4,1},
        {Tune::D4,2},  {Tune::C4,0.5},{Tune::D4,0.5},{Tune::E4,3}
    };
    PlayMusic(music,200);
    return *this;
}

};