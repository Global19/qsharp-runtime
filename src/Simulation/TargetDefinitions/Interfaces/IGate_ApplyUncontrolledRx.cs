// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Intrinsic.Interfaces
{
    public interface IGate_ApplyUncontrolledRx : IOperationFactory
    {
        void ApplyUncontrolledRx__Body(double angle, Qubit target);

        void ApplyUncontrolledRx__AdjointBody(double angle, Qubit target);
    }
}