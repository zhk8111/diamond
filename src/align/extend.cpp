/****
DIAMOND protein aligner
Copyright (C) 2013-2020 Max Planck Society for the Advancement of Science e.V.
                        Benjamin Buchfink
                        Eberhard Karls Universitaet Tuebingen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/

#include "extend.h"
#include "../data/queries.h"
#include "../basic/config.h"
#include "../dp/comp_based_stats.h"
#include "target.h"
#include "../dp/dp.h"

using std::vector;
using std::list;
using std::array;

namespace Extension {

void extend(const Parameters &params, size_t query_id, Trace_pt_list::iterator begin, Trace_pt_list::iterator end, const Metadata &metadata, Statistics &stat) {
	const unsigned contexts = align_mode.query_contexts;
	vector<sequence> query_seq;
	vector<Bias_correction> query_cb;

	if (config.log_query)
		cout << "Query = " << query_ids::get()[query_id].c_str() << endl;

	for (unsigned i = 0; i < contexts; ++i)
		query_seq.push_back(query_seqs::get()[query_id*contexts + i]);

	if (config.comp_based_stats == 1)
		for (unsigned i = 0; i < contexts; ++i)
			query_cb.emplace_back(query_seq[i]);

	vector<WorkTarget> targets = ungapped_stage(query_seq.data(), query_cb.data(), begin, end);
	stat.inc(Statistics::TARGET_HITS0, targets.size());

	rank_targets(targets, config.rank_ratio == -1 ? (query_seq[0].length() > 50 ? 0.6 : 0.9) : config.rank_ratio, config.rank_factor == -1 ? 1e3 : config.rank_factor);
	stat.inc(Statistics::TARGET_HITS1, targets.size());

	array<vector<DpTarget>, MAX_CONTEXT> dp_targets;

}

}