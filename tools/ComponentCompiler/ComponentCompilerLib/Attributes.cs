using System;

namespace ComponentCompilerLib
{
	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
	public class ComponentAttribute : System.Attribute
	{
		public ComponentAttribute()
		{
			OnePerEntity = true;
		}

		public bool OnePerEntity {
			get;
			set;
		}

		public int SpawnOrder {
			get;
			set;
		}
	}

	[AttributeUsage(AttributeTargets.Property)]
	public class ComponentFieldAttribute : System.Attribute
	{
		public int Tag {
			get;
			set;
		}

		public int FixedSize {
			get;
			set;
		}
	}
}
