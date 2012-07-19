using System;
using System.Net;
using System.Globalization;

namespace ACFramework
{
    public struct Plane3D : IEquatable<Plane3D>
    {
        public Vector3 Normal;
        public float D;
        public Plane3D(float a, float b, float c, float d)
        {
            Normal = Vector3.Zero;
            this.Normal.X = a;
            this.Normal.Y = b;
            this.Normal.Z = c;
            this.D = d;
        }

        public Plane3D(Vector3 normal, float d)
        {
            this.Normal = normal;
            this.D = d;
        }

        public Plane3D(Vector4 value)
        {
            this.Normal = Vector3.Zero;
            this.Normal.X = value.X;
            this.Normal.Y = value.Y;
            this.Normal.Z = value.Z;
            this.D = value.W;
        }

        public Plane3D(Vector3 point1, Vector3 point2, Vector3 point3)
        {
            this.Normal = Vector3.Zero;
            float num10 = point2.X - point1.X;
            float num9 = point2.Y - point1.Y;
            float num8 = point2.Z - point1.Z;
            float num7 = point3.X - point1.X;
            float num6 = point3.Y - point1.Y;
            float num5 = point3.Z - point1.Z;
            float num4 = (num9 * num5) - (num8 * num6);
            float num3 = (num8 * num7) - (num10 * num5);
            float num2 = (num10 * num6) - (num9 * num7);
            float num11 = ((num4 * num4) + (num3 * num3)) + (num2 * num2);
            float num = 1f / ((float)Math.Sqrt((double)num11));
            this.Normal.X = num4 * num;
            this.Normal.Y = num3 * num;
            this.Normal.Z = num2 * num;
            this.D = -(((this.Normal.X * point1.X) + (this.Normal.Y * point1.Y)) + (this.Normal.Z * point1.Z));
        }

        public bool Equals(Plane3D other)
        {
            return ((((this.Normal.X == other.Normal.X) && (this.Normal.Y == other.Normal.Y)) && (this.Normal.Z == other.Normal.Z)) && (this.D == other.D));
        }

        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Plane3D)
            {
                flag = this.Equals((Plane3D)obj);
            }
            return flag;
        }

        public override int GetHashCode()
        {
            return (this.Normal.GetHashCode() + this.D.GetHashCode());
        }

        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "{{Normal:{0} D:{1}}}", new object[] { this.Normal.ToString(), this.D.ToString(currentCulture) });
        }

        public void Normalize()
        {
            float num2 = ((this.Normal.X * this.Normal.X) + (this.Normal.Y * this.Normal.Y)) + (this.Normal.Z * this.Normal.Z);
            if (Math.Abs((float)(num2 - 1f)) >= 1.192093E-07f)
            {
                float num = 1f / ((float)Math.Sqrt((double)num2));
                this.Normal.X *= num;
                this.Normal.Y *= num;
                this.Normal.Z *= num;
                this.D *= num;
            }
        }

        public static Plane3D Normalize(Plane3D value)
        {
            Plane3D plane;
            plane.Normal = Vector3.Zero;
            float num2 = ((value.Normal.X * value.Normal.X) + (value.Normal.Y * value.Normal.Y)) + (value.Normal.Z * value.Normal.Z);
            if (Math.Abs((float)(num2 - 1f)) < 1.192093E-07f)
            {
                plane.Normal = value.Normal;
                plane.D = value.D;
                return plane;
            }
            float num = 1f / ((float)Math.Sqrt((double)num2));
            plane.Normal.X = value.Normal.X * num;
            plane.Normal.Y = value.Normal.Y * num;
            plane.Normal.Z = value.Normal.Z * num;
            plane.D = value.D * num;
            return plane;
        }

        public static void Normalize(ref Plane3D value, out Plane3D result)
        {
            float num2 = ((value.Normal.X * value.Normal.X) + (value.Normal.Y * value.Normal.Y)) + (value.Normal.Z * value.Normal.Z);
            if (Math.Abs((float)(num2 - 1f)) < 1.192093E-07f)
            {
                result.Normal = value.Normal;
                result.D = value.D;
            }
            else
            {
                float num = 1f / ((float)Math.Sqrt((double)num2));
                result.Normal = Vector3.Zero;
                result.Normal.X = value.Normal.X * num;
                result.Normal.Y = value.Normal.Y * num;
                result.Normal.Z = value.Normal.Z * num;
                result.D = value.D * num;
            }
        }

        public static Plane3D Transform(Plane3D plane, Matrix matrix)
        {
            Plane3D plane2;
            Matrix matrix2;
            Matrix.Invert(ref matrix, out matrix2);
            float x = plane.Normal.X;
            float y = plane.Normal.Y;
            float z = plane.Normal.Z;
            float d = plane.D;
            plane2.Normal = Vector3.Zero;
            plane2.Normal.X = (((x * matrix2.M11) + (y * matrix2.M12)) + (z * matrix2.M13)) + (d * matrix2.M14);
            plane2.Normal.Y = (((x * matrix2.M21) + (y * matrix2.M22)) + (z * matrix2.M23)) + (d * matrix2.M24);
            plane2.Normal.Z = (((x * matrix2.M31) + (y * matrix2.M32)) + (z * matrix2.M33)) + (d * matrix2.M34);
            plane2.D = (((x * matrix2.M41) + (y * matrix2.M42)) + (z * matrix2.M43)) + (d * matrix2.M44);
            return plane2;
        }

        public static void Transform(ref Plane3D plane, ref Matrix matrix, out Plane3D result)
        {
            Matrix matrix2;
            Matrix.Invert(ref matrix, out matrix2);
            float x = plane.Normal.X;
            float y = plane.Normal.Y;
            float z = plane.Normal.Z;
            float d = plane.D;
            result.Normal = Vector3.Zero;
            result.Normal.X = (((x * matrix2.M11) + (y * matrix2.M12)) + (z * matrix2.M13)) + (d * matrix2.M14);
            result.Normal.Y = (((x * matrix2.M21) + (y * matrix2.M22)) + (z * matrix2.M23)) + (d * matrix2.M24);
            result.Normal.Z = (((x * matrix2.M31) + (y * matrix2.M32)) + (z * matrix2.M33)) + (d * matrix2.M34);
            result.D = (((x * matrix2.M41) + (y * matrix2.M42)) + (z * matrix2.M43)) + (d * matrix2.M44);
        }

        public static Plane3D Transform(Plane3D plane, Quaternion rotation)
        {
            Plane3D plane2;
            float num15 = rotation.X + rotation.X;
            float num5 = rotation.Y + rotation.Y;
            float num = rotation.Z + rotation.Z;
            float num14 = rotation.W * num15;
            float num13 = rotation.W * num5;
            float num12 = rotation.W * num;
            float num11 = rotation.X * num15;
            float num10 = rotation.X * num5;
            float num9 = rotation.X * num;
            float num8 = rotation.Y * num5;
            float num7 = rotation.Y * num;
            float num6 = rotation.Z * num;
            float num24 = (1f - num8) - num6;
            float num23 = num10 - num12;
            float num22 = num9 + num13;
            float num21 = num10 + num12;
            float num20 = (1f - num11) - num6;
            float num19 = num7 - num14;
            float num18 = num9 - num13;
            float num17 = num7 + num14;
            float num16 = (1f - num11) - num8;
            float x = plane.Normal.X;
            float y = plane.Normal.Y;
            float z = plane.Normal.Z;
            plane2.Normal = Vector3.Zero;
            plane2.Normal.X = ((x * num24) + (y * num23)) + (z * num22);
            plane2.Normal.Y = ((x * num21) + (y * num20)) + (z * num19);
            plane2.Normal.Z = ((x * num18) + (y * num17)) + (z * num16);
            plane2.D = plane.D;
            return plane2;
        }

        public static void Transform(ref Plane3D plane, ref Quaternion rotation, out Plane3D result)
        {
            float num15 = rotation.X + rotation.X;
            float num5 = rotation.Y + rotation.Y;
            float num = rotation.Z + rotation.Z;
            float num14 = rotation.W * num15;
            float num13 = rotation.W * num5;
            float num12 = rotation.W * num;
            float num11 = rotation.X * num15;
            float num10 = rotation.X * num5;
            float num9 = rotation.X * num;
            float num8 = rotation.Y * num5;
            float num7 = rotation.Y * num;
            float num6 = rotation.Z * num;
            float num24 = (1f - num8) - num6;
            float num23 = num10 - num12;
            float num22 = num9 + num13;
            float num21 = num10 + num12;
            float num20 = (1f - num11) - num6;
            float num19 = num7 - num14;
            float num18 = num9 - num13;
            float num17 = num7 + num14;
            float num16 = (1f - num11) - num8;
            float x = plane.Normal.X;
            float y = plane.Normal.Y;
            float z = plane.Normal.Z;
            result.Normal = Vector3.Zero;
            result.Normal.X = ((x * num24) + (y * num23)) + (z * num22);
            result.Normal.Y = ((x * num21) + (y * num20)) + (z * num19);
            result.Normal.Z = ((x * num18) + (y * num17)) + (z * num16);
            result.D = plane.D;
        }

        public float Dot(Vector4 value)
        {
            return ((((this.Normal.X * value.X) + (this.Normal.Y * value.Y)) + (this.Normal.Z * value.Z)) + (this.D * value.W));
        }

        public void Dot(ref Vector4 value, out float result)
        {
            result = (((this.Normal.X * value.X) + (this.Normal.Y * value.Y)) + (this.Normal.Z * value.Z)) + (this.D * value.W);
        }

        public float DotCoordinate(Vector3 value)
        {
            return ((((this.Normal.X * value.X) + (this.Normal.Y * value.Y)) + (this.Normal.Z * value.Z)) + this.D);
        }

        public void DotCoordinate(ref Vector3 value, out float result)
        {
            result = (((this.Normal.X * value.X) + (this.Normal.Y * value.Y)) + (this.Normal.Z * value.Z)) + this.D;
        }

        public float DotNormal(Vector3 value)
        {
            return (((this.Normal.X * value.X) + (this.Normal.Y * value.Y)) + (this.Normal.Z * value.Z));
        }

        public void DotNormal(ref Vector3 value, out float result)
        {
            result = ((this.Normal.X * value.X) + (this.Normal.Y * value.Y)) + (this.Normal.Z * value.Z);
        }

        public static bool operator ==(Plane3D lhs, Plane3D rhs)
        {
            return lhs.Equals(rhs);
        }

        public static bool operator !=(Plane3D lhs, Plane3D rhs)
        {
            if (((lhs.Normal.X == rhs.Normal.X) && (lhs.Normal.Y == rhs.Normal.Y)) && (lhs.Normal.Z == rhs.Normal.Z))
            {
                return (lhs.D != rhs.D);
            }
            return true;
        }
    }
}
