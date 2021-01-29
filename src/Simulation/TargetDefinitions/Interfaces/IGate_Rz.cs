// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Intrinsic.Interfaces
{
    public interface IGate_Rz : IOperationFactory
    {
        void Rz__Body(double angle, Qubit target);

        void Rz__AdjointBody(double angle, Qubit target);

        void Rz__ControlledBody(IQArray<Qubit> controls, double angle, Qubit target);

        void Rz__ControlledAdjointBody(IQArray<Qubit> controls, double angle, Qubit target);
    }
}