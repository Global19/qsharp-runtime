// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <assert.h>

#include "tracer.hpp"

using namespace std;

namespace Microsoft
{
namespace Quantum
{
    thread_local std::shared_ptr<CTracer> tracer = nullptr;
    std::shared_ptr<CTracer> CreateTracer()
    {
        tracer = std::make_shared<CTracer>();
        return tracer;
    }

    void CTracer::AddOperationToLayer(OpId id, size_t layer)
    {
        auto inserted = this->metricsByLayer[layer].operations.insert({id, 1});
        if (!inserted.second)
        {
            assert(inserted.first->first == id);
            inserted.first->second += 1;
        }
    }

    void CTracer::TraceSingleQubitOp(OpId id, Duration opDuration, Qubit target)
    {
        QubitState& qstate = this->qubits[reinterpret_cast<size_t>(target)];
        if (opDuration == 0)
        {
            if (qstate.layer != INVALID)
            {
                AddOperationToLayer(id, qstate.layer);
            }
            else
            {
                qstate.pendingZeroOps.push_back(id);
            }
        }
        else
        {
            // Figure out the layer this operation should go into.
            int layerToInsertInto = INVALID;
            if (qstate.layer != INVALID)
            {
                Layer& lastUsedIn = this->metricsByLayer[qstate.layer];
                if (qstate.lastUsedTime + opDuration <= lastUsedIn.startTime + lastUsedIn.duration)
                {
                    layerToInsertInto = qstate.layer;
                }
                else if (opDuration <= this->preferredLayerDuration && qstate.layer + 1 < this->metricsByLayer.size())
                {
                    layerToInsertInto = qstate.layer + 1;
                }
            }
            else if (opDuration <= this->preferredLayerDuration && !this->metricsByLayer.empty())
            {
                // the qubit hasn't been used in any of the layers yet -- add it to the first layer
                layerToInsertInto = 0;
            }

            if (layerToInsertInto == INVALID)
            {
                // Create a new layer for the operation.
                Time layerStartTime = 0;
                if (!this->metricsByLayer.empty())
                {
                    const Layer& lastLayer = this->metricsByLayer.back();
                    layerStartTime = lastLayer.startTime + lastLayer.duration;
                }
                this->metricsByLayer.push_back(Layer{max(this->preferredLayerDuration, opDuration), layerStartTime});
                layerToInsertInto = static_cast<int>(this->metricsByLayer.size()) - 1;
            }

            // Add the operation and the pending zero-duration ones into the layer.
            AddOperationToLayer(id, layerToInsertInto);
            for (OpId idPending : qstate.pendingZeroOps)
            {
                AddOperationToLayer(idPending, layerToInsertInto);
            }

            // Update the qubit state.
            qstate.layer = layerToInsertInto;
            const Time layerStart = this->metricsByLayer[layerToInsertInto].startTime;
            qstate.lastUsedTime = max(layerStart, qstate.lastUsedTime) + opDuration;
            qstate.pendingZeroOps.clear();
        }
    }
    void CTracer::TraceControlledSingleQubitOp(int32_t id, int32_t duration, int64_t nCtrls, Qubit* ctls, Qubit target)
    {
    }
} // namespace Quantum
} // namespace Microsoft