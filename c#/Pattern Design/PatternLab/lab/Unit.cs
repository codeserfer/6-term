using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpecialUnits;

namespace lab
{

    public interface IClonableUnit
    {
        Unit Clone();
    }

    public interface ICanBeHealed
    {
        Int32 maxHealth { get; }
        void Heal(Int32 count);
    }

    public interface ISpecialAbility
    {
        void SpecialAbility();
    }

    public class Unit
    {
        public readonly UnitType type;
        public readonly Int32 id;
        public readonly Int32 strenght;
        public readonly Int32 defense;
        public readonly Int32 cost;
        public readonly Boolean isTreatable;
        public readonly Boolean clonable;

        public Boolean AreDeath()
        {
            return health <= 0;
        }

        public void Melee(Int32 enemyStrenght)
        {
            var rand = new Random();

            var damage = (Int32)(enemyStrenght*rand.NextDouble() - defense*rand.NextDouble());

            if (damage < 0) damage = 0;

            health -= damage; 
        }

        public Int32 health { get; protected set; }

        public Unit(
                UnitType type, 
                Int32 id,
                Int32 health,
                Int32 strenght, 
                Int32 defense, 
                Int32 cost, 
                Boolean isTreatable,
                Boolean clonable
                    )
        {
            this.type = type;
            this.id = id;
            this.health = health;
            this.strenght = strenght;
            this.defense = defense;
            this.cost = cost;
            this.isTreatable = isTreatable;
            this.clonable = clonable;
        }
    }


    public class Healer : Unit, ICanBeHealed, ISpecialAbility
    {
        public Healer(Int32 id)
            : base(
                UnitType.light,
                id,
                Defaults.Healer.health,
                Defaults.Healer.strenght,
                Defaults.Healer.defense,
                Defaults.Healer.cost,
                Defaults.Healer.treatable,
                Defaults.Healer.clonable
                ){}

        public Unit Clone()
        {
            return (Healer) this.MemberwiseClone();
        }

        public int maxHealth
        {
            get { return Defaults.Healer.health; }
            private set { }
        }

        public void Heal(int count)
        {
            this.health += count;

            if (this.health > maxHealth) this.health = maxHealth;
        }

        public void SpecialAbility()
        {
            throw new NotImplementedException();
        }
    }

    public class Infantryman : Unit, IClonableUnit, ICanBeHealed, ISpecialAbility
    {
        public Infantryman(Int32 id)
            : base(
                Defaults.Infantryman.type,
                id,
                Defaults.Infantryman.health,
                Defaults.Infantryman.strenght,
                Defaults.Infantryman.defense,
                Defaults.Infantryman.cost,
                Defaults.Infantryman.treatable,
                Defaults.Infantryman.clonable
                ) { }

        public Unit Clone()
        {
            return (Infantryman)this.MemberwiseClone();
        }

        public int maxHealth
        {
            get { return Defaults.Infantryman.health; }
            private set { }
        }

        public void Heal(int count)
        {
            this.health += count;

            if (this.health > maxHealth) this.health = maxHealth;
        }

        public void SpecialAbility()
        {
            throw new NotImplementedException();
        }
    }

    public class Archer : Unit, IClonableUnit, ICanBeHealed
    {
        public Archer(Int32 id)
            : base(
                Defaults.Archer.type,
                id,
                Defaults.Archer.health,
                Defaults.Archer.strenght,
                Defaults.Archer.defense,
                Defaults.Archer.cost,
                Defaults.Archer.treatable,
                Defaults.Archer.clonable
                ) { }

        public Unit Clone()
        {
            return (Archer)this.MemberwiseClone();
        }

        public int maxHealth
        {
            get { return Defaults.Archer.health; }
            private set { }
        }

        public void Heal(int count)
        {
            this.health += count;

            if (this.health > maxHealth) this.health = maxHealth;
        }
    }

    public class Wizard : Unit, ISpecialAbility
    {
        public Wizard(Int32 id)
            : base(
                Defaults.Wizard.type,
                id,
                Defaults.Wizard.health,
                Defaults.Wizard.strenght,
                Defaults.Wizard.defense,
                Defaults.Wizard.cost,
                Defaults.Wizard.treatable,
                Defaults.Wizard.clonable
                ) { }

        public Unit Clone()
        {
            return (Wizard)this.MemberwiseClone();
        }

        public void SpecialAbility()
        {
            throw new NotImplementedException();
        }
    }

    public class Knight : Unit, IClonableUnit
    {
        public Knight(Int32 id)
            : base(
                Defaults.Knight.type,
                id,
                Defaults.Knight.health,
                Defaults.Knight.strenght,
                Defaults.Knight.defense,
                Defaults.Knight.cost,
                Defaults.Knight.treatable,
                Defaults.Knight.clonable
                ) { }
        public Unit Clone()
        {
            return (Knight)this.MemberwiseClone();
        }
    }

    

    //Class for Barrier is in BarrierAdapter

}