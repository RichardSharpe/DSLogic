/*
 * This file is part of the DSLogic-gui project.
 * DSLogic-gui is based on PulseView.
 *
 * Copyright (C) 2012 Joel Holdsworth <joel@airwebreathe.org.uk>
 * Copyright (C) 2013 DreamSourceLab <dreamsourcelab@dreamsourcelab.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */


#ifndef DSLOGIC_PV_DS1WIRE_H
#define DSLOGIC_PV_DS1WIRE_H

#include "decoder.h"
#include "../data/logic.h"
#include "../data/logicsnapshot.h"

#include <boost/shared_ptr.hpp>

namespace pv {

namespace data {
class LogicSnapshot;
}

namespace decoder {

class ds1Wire : public Decoder
{
private:
    static const int TableSize = 8;
    static const QColor ColorTable[TableSize];
    static const QString StateTable[TableSize];

    enum {Unknown = 0, Reset, Presence, Command, Family, Serial, Crc, Data};

private:

    double get_next_pulse_width(bool level,
                                uint64_t samplerate, uint64_t &left, uint64_t right,
                                const boost::shared_ptr<data::LogicSnapshot> &snapshot);

    uint8_t get_next_data(bool speed, bool &valid, uint64_t &start, uint64_t &end,
                          uint64_t samplerate, uint64_t &left, uint64_t right,
                          const boost::shared_ptr<data::LogicSnapshot> &snapshot);

public:
    ds1Wire(boost::shared_ptr<pv::data::Logic> data,
        std::list<int> _sel_probes, QMap<QString, QVariant> &_options, QMap<QString, int> _options_index);

    virtual ~ds1Wire();

    QString get_decode_name();

    void recode(std::list <int > _sel_probes, QMap <QString, QVariant>& _options, QMap <QString, int> _options_index);

    void decode();

    void fill_color_table(std::vector <QColor>& _color_table);

    void fill_state_table(std::vector <QString>& _state_table);

    void get_subsampled_states(std::vector<struct ds_view_state> &states,
                               uint64_t start, uint64_t end,
                               float min_length);

private:

    int _wire_index;
    uint64_t _max_width;
    std::vector< pv::data::LogicSnapshot::EdgePair > _cur_edges;
    std::vector< std::pair<std::pair<uint64_t, uint64_t>, std::pair<uint8_t, uint8_t> > > _state_index;
};

} // namespace decoder
} // namespace pv

#endif // DSLOGIC_PV_DS1WIRE_H
