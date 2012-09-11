﻿using System;
using System.Globalization;

namespace EngineMathCLI
{
    public class BoundingFrustum : IEquatable<BoundingFrustum>
    {
        // Fields
        private const int BottomPlaneIndex = 5;
        internal Vector3[] cornerArray;
        public const int CornerCount = 8;
        private const int FarPlaneIndex = 1;
        private Gjk gjk;
        private const int LeftPlaneIndex = 2;
        private Matrix matrix;
        private const int NearPlaneIndex = 0;
        private const int NumPlanes = 6;
        private Plane[] planes;
        private const int RightPlaneIndex = 3;
        private const int TopPlaneIndex = 4;

        // Methods
        private BoundingFrustum()
        {
            this.planes = new Plane[6];
            this.cornerArray = new Vector3[8];
        }

        public BoundingFrustum(Matrix value)
        {
            this.planes = new Plane[6];
            this.cornerArray = new Vector3[8];
            this.SetMatrix(ref value);
        }

        private static Vector3 ComputeIntersection(ref Plane plane, ref Ray ray)
        {
            float num = (-plane.D - Vector3.Dot(plane.Normal, ray.Position)) / Vector3.Dot(plane.Normal, ray.Direction);
            return (ray.Position + ((Vector3)(ray.Direction * num)));
        }

        private static Ray ComputeIntersectionLine(ref Plane p1, ref Plane p2)
        {
            Ray ray = new Ray
            {
                Direction = Vector3.Cross(p1.Normal, p2.Normal)
            };
            float num = ray.Direction.LengthSquared();
            ray.Position = (Vector3)(Vector3.Cross((Vector3)((-p1.D * p2.Normal) + (p2.D * p1.Normal)), ray.Direction) / num);
            return ray;
        }

        public ContainmentType Contains(BoundingBox box)
        {
            bool flag = false;
            foreach (Plane plane in this.planes)
            {
                switch (box.Intersects(plane))
                {
                    case PlaneIntersectionType.Front:
                        return ContainmentType.Disjoint;

                    case PlaneIntersectionType.Intersecting:
                        flag = true;
                        break;
                }
            }
            if (!flag)
            {
                return ContainmentType.Contains;
            }
            return ContainmentType.Intersects;
        }

        public ContainmentType Contains(BoundingFrustum frustum)
        {
            if (frustum == null)
            {
                throw new ArgumentNullException("frustum");
            }
            ContainmentType disjoint = ContainmentType.Disjoint;
            if (this.Intersects(frustum))
            {
                disjoint = ContainmentType.Contains;
                for (int i = 0; i < this.cornerArray.Length; i++)
                {
                    if (this.Contains(frustum.cornerArray[i]) == ContainmentType.Disjoint)
                    {
                        return ContainmentType.Intersects;
                    }
                }
            }
            return disjoint;
        }

        public ContainmentType Contains(BoundingSphere sphere)
        {
            Vector3 center = sphere.Center;
            float radius = sphere.Radius;
            int num2 = 0;
            foreach (Plane plane in this.planes)
            {
                float num5 = ((plane.Normal.X * center.X) + (plane.Normal.Y * center.Y)) + (plane.Normal.Z * center.Z);
                float num3 = num5 + plane.D;
                if (num3 > radius)
                {
                    return ContainmentType.Disjoint;
                }
                if (num3 < -radius)
                {
                    num2++;
                }
            }
            if (num2 != 6)
            {
                return ContainmentType.Intersects;
            }
            return ContainmentType.Contains;
        }

        public ContainmentType Contains(Vector3 point)
        {
            foreach (Plane plane in this.planes)
            {
                float num2 = (((plane.Normal.X * point.X) + (plane.Normal.Y * point.Y)) + (plane.Normal.Z * point.Z)) + plane.D;
                if (num2 > 1E-05f)
                {
                    return ContainmentType.Disjoint;
                }
            }
            return ContainmentType.Contains;
        }

        public void Contains(ref BoundingBox box, out ContainmentType result)
        {
            bool flag = false;
            foreach (Plane plane in this.planes)
            {
                switch (box.Intersects(plane))
                {
                    case PlaneIntersectionType.Front:
                        result = ContainmentType.Disjoint;
                        return;

                    case PlaneIntersectionType.Intersecting:
                        flag = true;
                        break;
                }
            }
            result = flag ? ContainmentType.Intersects : ContainmentType.Contains;
        }

        public void Contains(ref BoundingSphere sphere, out ContainmentType result)
        {
            Vector3 center = sphere.Center;
            float radius = sphere.Radius;
            int num2 = 0;
            foreach (Plane plane in this.planes)
            {
                float num5 = ((plane.Normal.X * center.X) + (plane.Normal.Y * center.Y)) + (plane.Normal.Z * center.Z);
                float num3 = num5 + plane.D;
                if (num3 > radius)
                {
                    result = ContainmentType.Disjoint;
                    return;
                }
                if (num3 < -radius)
                {
                    num2++;
                }
            }
            result = (num2 == 6) ? ContainmentType.Contains : ContainmentType.Intersects;
        }

        public void Contains(ref Vector3 point, out ContainmentType result)
        {
            foreach (Plane plane in this.planes)
            {
                float num2 = (((plane.Normal.X * point.X) + (plane.Normal.Y * point.Y)) + (plane.Normal.Z * point.Z)) + plane.D;
                if (num2 > 1E-05f)
                {
                    result = ContainmentType.Disjoint;
                    return;
                }
            }
            result = ContainmentType.Contains;
        }

        public bool Equals(BoundingFrustum other)
        {
            if (other == null)
            {
                return false;
            }
            return (this.matrix == other.matrix);
        }

        public override bool Equals(object obj)
        {
            bool flag = false;
            BoundingFrustum frustum = obj as BoundingFrustum;
            if (frustum != null)
            {
                flag = this.matrix == frustum.matrix;
            }
            return flag;
        }

        public Vector3[] GetCorners()
        {
            return (Vector3[])this.cornerArray.Clone();
        }

        public void GetCorners(Vector3[] corners)
        {
            if (corners == null)
            {
                throw new ArgumentNullException("corners");
            }
            if (corners.Length < 8)
            {
                throw new ArgumentOutOfRangeException("corners", FrameworkResources.NotEnoughCorners);
            }
            this.cornerArray.CopyTo(corners, 0);
        }

        public override int GetHashCode()
        {
            return this.matrix.GetHashCode();
        }

        public bool Intersects(BoundingBox box)
        {
            bool flag;
            this.Intersects(ref box, out flag);
            return flag;
        }

        public bool Intersects(BoundingFrustum frustum)
        {
            Vector3 closestPoint;
            if (frustum == null)
            {
                throw new ArgumentNullException("frustum");
            }
            if (this.gjk == null)
            {
                this.gjk = new Gjk();
            }
            this.gjk.Reset();
            Vector3.Subtract(ref this.cornerArray[0], ref frustum.cornerArray[0], out closestPoint);
            if (closestPoint.LengthSquared() < 1E-05f)
            {
                Vector3.Subtract(ref this.cornerArray[0], ref frustum.cornerArray[1], out closestPoint);
            }
            float maxValue = float.MaxValue;
            float num3 = 0f;
            do
            {
                Vector3 vector2 = Vector3.Zero;
                Vector3 vector3 = Vector3.Zero;
                Vector3 vector4 = Vector3.Zero;
                Vector3 vector5 = Vector3.Zero;
                vector5.X = -closestPoint.X;
                vector5.Y = -closestPoint.Y;
                vector5.Z = -closestPoint.Z;
                this.SupportMapping(ref vector5, out vector4);
                frustum.SupportMapping(ref closestPoint, out vector3);
                Vector3.Subtract(ref vector4, ref vector3, out vector2);
                float num4 = ((closestPoint.X * vector2.X) + (closestPoint.Y * vector2.Y)) + (closestPoint.Z * vector2.Z);
                if (num4 > 0f)
                {
                    return false;
                }
                this.gjk.AddSupportPoint(ref vector2);
                closestPoint = this.gjk.ClosestPoint;
                float num2 = maxValue;
                maxValue = closestPoint.LengthSquared();
                num3 = 4E-05f * this.gjk.MaxLengthSquared;
                if ((num2 - maxValue) <= (1E-05f * num2))
                {
                    return false;
                }
            }
            while (!this.gjk.FullSimplex && (maxValue >= num3));
            return true;
        }

        public bool Intersects(BoundingSphere sphere)
        {
            bool flag;
            this.Intersects(ref sphere, out flag);
            return flag;
        }

        public PlaneIntersectionType Intersects(Plane plane)
        {
            int num = 0;
            for (int i = 0; i < 8; i++)
            {
                float num3;
                Vector3.Dot(ref this.cornerArray[i], ref plane.Normal, out num3);
                if ((num3 + plane.D) > 0f)
                {
                    num |= 1;
                }
                else
                {
                    num |= 2;
                }
                if (num == 3)
                {
                    return PlaneIntersectionType.Intersecting;
                }
            }
            if (num != 1)
            {
                return PlaneIntersectionType.Back;
            }
            return PlaneIntersectionType.Front;
        }

        public float? Intersects(Ray ray)
        {
            float? nullable;
            this.Intersects(ref ray, out nullable);
            return nullable;
        }

        public void Intersects(ref BoundingBox box, out bool result)
        {
            Vector3 closestPoint;
            Vector3 vector2 = Vector3.Zero;
            Vector3 vector3 = Vector3.Zero;
            Vector3 vector4 = Vector3.Zero;
            Vector3 vector5 = Vector3.Zero;
            if (this.gjk == null)
            {
                this.gjk = new Gjk();
            }
            this.gjk.Reset();
            Vector3.Subtract(ref this.cornerArray[0], ref box.Min, out closestPoint);
            if (closestPoint.LengthSquared() < 1E-05f)
            {
                Vector3.Subtract(ref this.cornerArray[0], ref box.Max, out closestPoint);
            }
            float maxValue = float.MaxValue;
            float num3 = 0f;
            result = false;
        Label_006D:
            vector5.X = -closestPoint.X;
            vector5.Y = -closestPoint.Y;
            vector5.Z = -closestPoint.Z;
            this.SupportMapping(ref vector5, out vector4);
            box.SupportMapping(ref closestPoint, out vector3);
            Vector3.Subtract(ref vector4, ref vector3, out vector2);
            float num4 = ((closestPoint.X * vector2.X) + (closestPoint.Y * vector2.Y)) + (closestPoint.Z * vector2.Z);
            if (num4 <= 0f)
            {
                this.gjk.AddSupportPoint(ref vector2);
                closestPoint = this.gjk.ClosestPoint;
                float num2 = maxValue;
                maxValue = closestPoint.LengthSquared();
                if ((num2 - maxValue) > (1E-05f * num2))
                {
                    num3 = 4E-05f * this.gjk.MaxLengthSquared;
                    if (!this.gjk.FullSimplex && (maxValue >= num3))
                    {
                        goto Label_006D;
                    }
                    result = true;
                }
            }
        }

        public void Intersects(ref BoundingSphere sphere, out bool result)
        {
            Vector3 unitX;
            Vector3 vector2 = Vector3.Zero;
            Vector3 vector3 = Vector3.Zero;
            Vector3 vector4 = Vector3.Zero;
            Vector3 vector5 = Vector3.Zero;
            if (this.gjk == null)
            {
                this.gjk = new Gjk();
            }
            this.gjk.Reset();
            Vector3.Subtract(ref this.cornerArray[0], ref sphere.Center, out unitX);
            if (unitX.LengthSquared() < 1E-05f)
            {
                unitX = Vector3.UnitX;
            }
            float maxValue = float.MaxValue;
            float num3 = 0f;
            result = false;
        Label_005A:
            vector5.X = -unitX.X;
            vector5.Y = -unitX.Y;
            vector5.Z = -unitX.Z;
            this.SupportMapping(ref vector5, out vector4);
            sphere.SupportMapping(ref unitX, out vector3);
            Vector3.Subtract(ref vector4, ref vector3, out vector2);
            float num4 = ((unitX.X * vector2.X) + (unitX.Y * vector2.Y)) + (unitX.Z * vector2.Z);
            if (num4 <= 0f)
            {
                this.gjk.AddSupportPoint(ref vector2);
                unitX = this.gjk.ClosestPoint;
                float num2 = maxValue;
                maxValue = unitX.LengthSquared();
                if ((num2 - maxValue) > (1E-05f * num2))
                {
                    num3 = 4E-05f * this.gjk.MaxLengthSquared;
                    if (!this.gjk.FullSimplex && (maxValue >= num3))
                    {
                        goto Label_005A;
                    }
                    result = true;
                }
            }
        }

        public void Intersects(ref Plane plane, out PlaneIntersectionType result)
        {
            int num = 0;
            for (int i = 0; i < 8; i++)
            {
                float num3;
                Vector3.Dot(ref this.cornerArray[i], ref plane.Normal, out num3);
                if ((num3 + plane.D) > 0f)
                {
                    num |= 1;
                }
                else
                {
                    num |= 2;
                }
                if (num == 3)
                {
                    result = PlaneIntersectionType.Intersecting;
                    return;
                }
            }
            result = (num == 1) ? PlaneIntersectionType.Front : PlaneIntersectionType.Back;
        }

        public void Intersects(ref Ray ray, out float? result)
        {
            ContainmentType type;
            this.Contains(ref ray.Position, out type);
            if (type == ContainmentType.Contains)
            {
                result = 0f;
            }
            else
            {
                float minValue = float.MinValue;
                float maxValue = float.MaxValue;
                result = 0;
                foreach (Plane plane in this.planes)
                {
                    float num3;
                    float num6;
                    Vector3 normal = plane.Normal;
                    Vector3.Dot(ref ray.Direction, ref normal, out num6);
                    Vector3.Dot(ref ray.Position, ref normal, out num3);
                    num3 += plane.D;
                    if (Math.Abs(num6) < 1E-05f)
                    {
                        if (num3 > 0f)
                        {
                            return;
                        }
                    }
                    else
                    {
                        float num = -num3 / num6;
                        if (num6 < 0f)
                        {
                            if (num > maxValue)
                            {
                                return;
                            }
                            if (num > minValue)
                            {
                                minValue = num;
                            }
                        }
                        else
                        {
                            if (num < minValue)
                            {
                                return;
                            }
                            if (num < maxValue)
                            {
                                maxValue = num;
                            }
                        }
                    }
                }
                float num7 = (minValue >= 0f) ? minValue : maxValue;
                if (num7 >= 0f)
                {
                    result = new float?(num7);
                }
            }
        }

        public static bool operator ==(BoundingFrustum a, BoundingFrustum b)
        {
            return object.Equals(a, b);
        }

        public static bool operator !=(BoundingFrustum a, BoundingFrustum b)
        {
            return !object.Equals(a, b);
        }

        private void SetMatrix(ref Matrix value)
        {
            this.matrix = value;
            this.planes[2].Normal.X = -value.M14 - value.M11;
            this.planes[2].Normal.Y = -value.M24 - value.M21;
            this.planes[2].Normal.Z = -value.M34 - value.M31;
            this.planes[2].D = -value.M44 - value.M41;
            this.planes[3].Normal.X = -value.M14 + value.M11;
            this.planes[3].Normal.Y = -value.M24 + value.M21;
            this.planes[3].Normal.Z = -value.M34 + value.M31;
            this.planes[3].D = -value.M44 + value.M41;
            this.planes[4].Normal.X = -value.M14 + value.M12;
            this.planes[4].Normal.Y = -value.M24 + value.M22;
            this.planes[4].Normal.Z = -value.M34 + value.M32;
            this.planes[4].D = -value.M44 + value.M42;
            this.planes[5].Normal.X = -value.M14 - value.M12;
            this.planes[5].Normal.Y = -value.M24 - value.M22;
            this.planes[5].Normal.Z = -value.M34 - value.M32;
            this.planes[5].D = -value.M44 - value.M42;
            this.planes[0].Normal.X = -value.M13;
            this.planes[0].Normal.Y = -value.M23;
            this.planes[0].Normal.Z = -value.M33;
            this.planes[0].D = -value.M43;
            this.planes[1].Normal.X = -value.M14 + value.M13;
            this.planes[1].Normal.Y = -value.M24 + value.M23;
            this.planes[1].Normal.Z = -value.M34 + value.M33;
            this.planes[1].D = -value.M44 + value.M43;
            for (int i = 0; i < 6; i++)
            {
                float num2 = this.planes[i].Normal.Length();
                this.planes[i].Normal = (Vector3)(this.planes[i].Normal / num2);
                this.planes[i].D /= num2;
            }
            Ray ray = ComputeIntersectionLine(ref this.planes[0], ref this.planes[2]);
            this.cornerArray[0] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[3] = ComputeIntersection(ref this.planes[5], ref ray);
            ray = ComputeIntersectionLine(ref this.planes[3], ref this.planes[0]);
            this.cornerArray[1] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[2] = ComputeIntersection(ref this.planes[5], ref ray);
            ray = ComputeIntersectionLine(ref this.planes[2], ref this.planes[1]);
            this.cornerArray[4] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[7] = ComputeIntersection(ref this.planes[5], ref ray);
            ray = ComputeIntersectionLine(ref this.planes[1], ref this.planes[3]);
            this.cornerArray[5] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[6] = ComputeIntersection(ref this.planes[5], ref ray);
        }

        internal void SupportMapping(ref Vector3 v, out Vector3 result)
        {
            float num3;
            int index = 0;
            Vector3.Dot(ref this.cornerArray[0], ref v, out num3);
            for (int i = 1; i < this.cornerArray.Length; i++)
            {
                float num2;
                Vector3.Dot(ref this.cornerArray[i], ref v, out num2);
                if (num2 > num3)
                {
                    index = i;
                    num3 = num2;
                }
            }
            result = this.cornerArray[index];
        }

        public override string ToString()
        {
            return string.Format(CultureInfo.CurrentCulture, "{{Near:{0} Far:{1} Left:{2} Right:{3} Top:{4} Bottom:{5}}}", new object[] { this.Near.ToString(), this.Far.ToString(), this.Left.ToString(), this.Right.ToString(), this.Top.ToString(), this.Bottom.ToString() });
        }

        // Properties
        public Plane Bottom
        {
            get
            {
                return this.planes[5];
            }
        }

        public Plane Far
        {
            get
            {
                return this.planes[1];
            }
        }

        public Plane Left
        {
            get
            {
                return this.planes[2];
            }
        }

        public Matrix Matrix
        {
            get
            {
                return this.matrix;
            }
            set
            {
                this.SetMatrix(ref value);
            }
        }

        public Plane Near
        {
            get
            {
                return this.planes[0];
            }
        }

        public Plane Right
        {
            get
            {
                return this.planes[3];
            }
        }

        public Plane Top
        {
            get
            {
                return this.planes[4];
            }
        }
    }
}
