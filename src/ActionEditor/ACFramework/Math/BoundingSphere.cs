using System;
using System.Net;
using System.Globalization;
using System.Collections.Generic;

namespace ACFramework
{
    public struct BoundingSphere : IEquatable<BoundingSphere>
    {
        public Vector3 Center;
        public float Radius;
        public BoundingSphere(Vector3 center, float radius)
        {
            if (radius < 0f)
            {
                throw new ArgumentException("Raio menor q zero");
            }
            this.Center = center;
            this.Radius = radius;
        }

        public bool Equals(BoundingSphere other)
        {
            return ((this.Center == other.Center) && (this.Radius == other.Radius));
        }

        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is BoundingSphere)
            {
                flag = this.Equals((BoundingSphere)obj);
            }
            return flag;
        }

        public override int GetHashCode()
        {
            return (this.Center.GetHashCode() + this.Radius.GetHashCode());
        }

        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "{{Center:{0} Radius:{1}}}", new object[] { this.Center.ToString(), this.Radius.ToString(currentCulture) });
        }

        public static BoundingSphere CreateMerged(BoundingSphere original, BoundingSphere additional)
        {
            BoundingSphere sphere;
            Vector3 vector2;
            Vector3.Subtract(ref additional.Center, ref original.Center, out vector2);
            float num = vector2.Length();
            float radius = original.Radius;
            float num2 = additional.Radius;
            if ((radius + num2) >= num)
            {
                if ((radius - num2) >= num)
                {
                    return original;
                }
                if ((num2 - radius) >= num)
                {
                    return additional;
                }
            }
            Vector3 vector = (Vector3)(vector2 * (1f / num));
            float num5 = MathHelper.Min(-radius, num - num2);
            float num4 = (MathHelper.Max(radius, num + num2) - num5) * 0.5f;
            sphere.Center = original.Center + ((Vector3)(vector * (num4 + num5)));
            sphere.Radius = num4;
            return sphere;
        }

        public static void CreateMerged(ref BoundingSphere original, ref BoundingSphere additional, out BoundingSphere result)
        {
            Vector3 vector2;
            Vector3.Subtract(ref additional.Center, ref original.Center, out vector2);
            float num = vector2.Length();
            float radius = original.Radius;
            float num2 = additional.Radius;
            if ((radius + num2) >= num)
            {
                if ((radius - num2) >= num)
                {
                    result = original;
                    return;
                }
                if ((num2 - radius) >= num)
                {
                    result = additional;
                    return;
                }
            }
            Vector3 vector = (Vector3)(vector2 * (1f / num));
            float num5 = MathHelper.Min(-radius, num - num2);
            float num4 = (MathHelper.Max(radius, num + num2) - num5) * 0.5f;
            result.Center = original.Center + ((Vector3)(vector * (num4 + num5)));
            result.Radius = num4;
        }

        public static BoundingSphere CreateFromBoundingBox(BoundingBox box)
        {
            float num;
            BoundingSphere sphere;
            Vector3.Lerp(ref box.Min, ref box.Max, 0.5f, out sphere.Center);
            Vector3.Distance(ref box.Min, ref box.Max, out num);
            sphere.Radius = num * 0.5f;
            return sphere;
        }

        public static void CreateFromBoundingBox(ref BoundingBox box, out BoundingSphere result)
        {
            float num;
            Vector3.Lerp(ref box.Min, ref box.Max, 0.5f, out result.Center);
            Vector3.Distance(ref box.Min, ref box.Max, out num);
            result.Radius = num * 0.5f;
        }

        public static BoundingSphere CreateFromPoints(IEnumerable<Vector3> points)
        {
            float num;
            float num2;
            Vector3 vector2;
            float num4;
            float num5;
            BoundingSphere sphere;
            Vector3 vector5;
            Vector3 vector6;
            Vector3 vector7;
            Vector3 vector8;
            Vector3 vector9;
            if (points == null)
            {
                throw new ArgumentNullException("points");
            }
            IEnumerator<Vector3> enumerator = points.GetEnumerator();
            if (!enumerator.MoveNext())
            {
                throw new ArgumentException(FrameworkResources.BoundingSphereZeroPoints);
            }
            Vector3 vector4 = vector5 = vector6 = vector7 = vector8 = vector9 = enumerator.Current;
            foreach (Vector3 vector in points)
            {
                if (vector.X < vector4.X)
                {
                    vector4 = vector;
                }
                if (vector.X > vector5.X)
                {
                    vector5 = vector;
                }
                if (vector.Y < vector6.Y)
                {
                    vector6 = vector;
                }
                if (vector.Y > vector7.Y)
                {
                    vector7 = vector;
                }
                if (vector.Z < vector8.Z)
                {
                    vector8 = vector;
                }
                if (vector.Z > vector9.Z)
                {
                    vector9 = vector;
                }
            }
            Vector3.Distance(ref vector5, ref vector4, out num5);
            Vector3.Distance(ref vector7, ref vector6, out num4);
            Vector3.Distance(ref vector9, ref vector8, out num2);
            if (num5 > num4)
            {
                if (num5 > num2)
                {
                    Vector3.Lerp(ref vector5, ref vector4, 0.5f, out vector2);
                    num = num5 * 0.5f;
                }
                else
                {
                    Vector3.Lerp(ref vector9, ref vector8, 0.5f, out vector2);
                    num = num2 * 0.5f;
                }
            }
            else if (num4 > num2)
            {
                Vector3.Lerp(ref vector7, ref vector6, 0.5f, out vector2);
                num = num4 * 0.5f;
            }
            else
            {
                Vector3.Lerp(ref vector9, ref vector8, 0.5f, out vector2);
                num = num2 * 0.5f;
            }
            foreach (Vector3 vector10 in points)
            {
                Vector3 vector3 = Vector3.Zero;
                vector3.X = vector10.X - vector2.X;
                vector3.Y = vector10.Y - vector2.Y;
                vector3.Z = vector10.Z - vector2.Z;
                float num3 = vector3.Length();
                if (num3 > num)
                {
                    num = (num + num3) * 0.5f;
                    vector2 += (Vector3)((1f - (num / num3)) * vector3);
                }
            }
            sphere.Center = vector2;
            sphere.Radius = num;
            return sphere;
        }

        public bool Intersects(BoundingBox box)
        {
            float num;
            Vector3 vector;
            Vector3.Clamp(ref this.Center, ref box.Min, ref box.Max, out vector);
            Vector3.DistanceSquared(ref this.Center, ref vector, out num);
            return (num <= (this.Radius * this.Radius));
        }

        public void Intersects(ref BoundingBox box, out bool result)
        {
            float num;
            Vector3 vector;
            Vector3.Clamp(ref this.Center, ref box.Min, ref box.Max, out vector);
            Vector3.DistanceSquared(ref this.Center, ref vector, out num);
            result = num <= (this.Radius * this.Radius);
        }

        public float? Intersects(Ray ray)
        {
            return ray.Intersects(this);
        }

        public void Intersects(ref Ray ray, out float? result)
        {
            ray.Intersects(ref this, out result);
        }

        public bool Intersects(BoundingSphere sphere)
        {
            float num3;
            Vector3.DistanceSquared(ref this.Center, ref sphere.Center, out num3);
            float radius = this.Radius;
            float num = sphere.Radius;
            if ((((radius * radius) + ((2f * radius) * num)) + (num * num)) <= num3)
            {
                return false;
            }
            return true;
        }

        public void Intersects(ref BoundingSphere sphere, out bool result)
        {
            float num3;
            Vector3.DistanceSquared(ref this.Center, ref sphere.Center, out num3);
            float radius = this.Radius;
            float num = sphere.Radius;
            result = (((radius * radius) + ((2f * radius) * num)) + (num * num)) > num3;
        }

        internal void SupportMapping(ref Vector3 v, out Vector3 result)
        {
            float num2 = v.Length();
            float num = this.Radius / num2;
            result = Vector3.Zero;
            result.X = this.Center.X + (v.X * num);
            result.Y = this.Center.Y + (v.Y * num);
            result.Z = this.Center.Z + (v.Z * num);
        }

        public BoundingSphere Transform(Matrix matrix)
        {
            BoundingSphere sphere = new BoundingSphere
            {
                Center = Vector3.Transform(this.Center, matrix)
            };
            float num4 = ((matrix.M11 * matrix.M11) + (matrix.M12 * matrix.M12)) + (matrix.M13 * matrix.M13);
            float num3 = ((matrix.M21 * matrix.M21) + (matrix.M22 * matrix.M22)) + (matrix.M23 * matrix.M23);
            float num2 = ((matrix.M31 * matrix.M31) + (matrix.M32 * matrix.M32)) + (matrix.M33 * matrix.M33);
            float num = Math.Max(num4, Math.Max(num3, num2));
            sphere.Radius = this.Radius * ((float)Math.Sqrt((double)num));
            return sphere;
        }

        public void Transform(ref Matrix matrix, out BoundingSphere result)
        {
            result.Center = Vector3.Transform(this.Center, matrix);
            float num4 = ((matrix.M11 * matrix.M11) + (matrix.M12 * matrix.M12)) + (matrix.M13 * matrix.M13);
            float num3 = ((matrix.M21 * matrix.M21) + (matrix.M22 * matrix.M22)) + (matrix.M23 * matrix.M23);
            float num2 = ((matrix.M31 * matrix.M31) + (matrix.M32 * matrix.M32)) + (matrix.M33 * matrix.M33);
            float num = Math.Max(num4, Math.Max(num3, num2));
            result.Radius = this.Radius * ((float)Math.Sqrt((double)num));
        }

        public static bool operator ==(BoundingSphere a, BoundingSphere b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(BoundingSphere a, BoundingSphere b)
        {
            if (!(a.Center != b.Center))
            {
                return (a.Radius != b.Radius);
            }
            return true;
        }
    }
}
