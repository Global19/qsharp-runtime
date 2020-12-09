// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Intrinsic.Interfaces
{
    public interface IGate_IsingZZ : IOperationFactory
    {
        void IsingZZ_Body(double angle, Qubit target1, Qubit target2);

        void IsingZZ_AdjointBody(double angle, Qubit target1, Qubit target2);

        void IsingZZ_ControlledBody(IQArray<Qubit> controls, double angle, Qubit target1, Qubit target2);

        void IsingZZ_ControlledAdjointBody(IQArray<Qubit> controls, double angle, Qubit target1, Qubit target2);
    }
}