/*
 * Copyright (c) 2012-2016 Daniele Bartolini and individual contributors.
 * License: https://github.com/taylor001/crown/blob/master/LICENSE-GPLv2
 */

namespace Crown
{
	public struct Matrix4x4
	{
		public Vector4 x;
		public Vector4 y;
		public Vector4 z;
		public Vector4 t;

		public Matrix4x4(Vector4 x, Vector4 y, Vector4 z, Vector4 t)
		{
			this.x = x;
			this.y = y;
			this.z = z;
			this.t = t;
		}
	}
}
